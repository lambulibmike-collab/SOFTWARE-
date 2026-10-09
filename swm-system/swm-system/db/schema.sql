CREATE EXTENSION IF NOT EXISTS postgis;
CREATE TABLE IF NOT EXISTS users (
  user_id BIGSERIAL PRIMARY KEY,
  user_name VARCHAR(100) NOT NULL UNIQUE,
  password_hash TEXT NOT NULL,
  role VARCHAR(20) NOT NULL CHECK (role IN ('admin','driver','technician')),
  created_at TIMESTAMPTZ NOT NULL DEFAULT now()
);
CREATE TABLE IF NOT EXISTS bin_nodes (
  node_id VARCHAR(50) PRIMARY KEY,
  lat DOUBLE PRECISION NOT NULL CHECK (lat BETWEEN -90 AND 90),
  lon DOUBLE PRECISION NOT NULL CHECK (lon BETWEEN -180 AND 180),
  geom geometry(Point,4326) GENERATED ALWAYS AS (ST_SetSRID(ST_MakePoint(lon,lat),4326)) STORED,
  height_cm REAL NOT NULL CHECK (height_cm > 20),
  fill_level_percent REAL CHECK (fill_level_percent BETWEEN 0 AND 100),
  battery_voltage REAL,
  last_seen TIMESTAMPTZ,
  created_at TIMESTAMPTZ NOT NULL DEFAULT now()
);
CREATE INDEX IF NOT EXISTS idx_bins_geom ON bin_nodes USING GIST(geom);
CREATE TABLE IF NOT EXISTS sensor_logs (
  log_id BIGSERIAL PRIMARY KEY,
  node_id VARCHAR(50) NOT NULL REFERENCES bin_nodes(node_id) ON DELETE CASCADE,
  logged_at TIMESTAMPTZ NOT NULL DEFAULT now(),
  raw_distance_cm REAL,
  battery_voltage REAL,
  fill_level_percent REAL CHECK (fill_level_percent BETWEEN 0 AND 100),
  received_at TIMESTAMPTZ NOT NULL DEFAULT now()
);
CREATE INDEX IF NOT EXISTS idx_logs_node_time ON sensor_logs(node_id,logged_at DESC);
CREATE TABLE IF NOT EXISTS vehicles (
  vehicle_id VARCHAR(50) PRIMARY KEY,
  capacity_kg REAL NOT NULL CHECK(capacity_kg > 0),
  start_lat DOUBLE PRECISION NOT NULL DEFAULT -15.4167,
  start_lon DOUBLE PRECISION NOT NULL DEFAULT 28.2833,
  status VARCHAR(20) NOT NULL DEFAULT 'available' CHECK(status IN ('available','busy','maintenance'))
);
CREATE TABLE IF NOT EXISTS routes (
  route_id UUID PRIMARY KEY,
  vehicle_id VARCHAR(50) REFERENCES vehicles(vehicle_id),
  target_bin_list JSONB NOT NULL DEFAULT '[]'::jsonb,
  total_distance_km REAL,
  estimated_time_min INTEGER,
  is_active BOOLEAN NOT NULL DEFAULT TRUE,
  created_at TIMESTAMPTZ NOT NULL DEFAULT now()
);
CREATE UNIQUE INDEX IF NOT EXISTS one_active_route ON routes(vehicle_id) WHERE is_active;
