# Smart Waste Monitoring & Dynamic Route Optimization

Starter implementation based on the supplied Chapter Four document. Includes a web dashboard, REST API, PostgreSQL/PostGIS schema, MQTT ingestion, Python CVRP heuristic, Docker Compose, and ESP32 firmware scaffold.

## Requirements
- Docker Engine + Docker Compose
- Python 3.10+ (for local route tests)
- Optional: PlatformIO + ESP32 board for hardware firmware

## Start the web app and backend
1. Copy `.env.example` to `.env` and change every password/secret.
2. Run `docker compose up --build -d` from this directory.
3. Open `http://localhost:8080`.
4. Demo logins are created only by the demo seed route when `DEMO_MODE=true`: `admin / ChangeMe123!`, `driver / ChangeMe123!`, `technician / ChangeMe123!`. Change/remove these before deployment.
5. API health: `http://localhost:3000/api/health`.

The dashboard can run with demo seed data. To use real MQTT telemetry, configure Mosquitto credentials/TLS and register each node in `bin_nodes` before sending to `swm/<node_id>/telemetry`.

## Run route tests locally
`cd routing && python run_tests.py`

## Important
This is a runnable starter, not proof of field readiness. Before production: enable HTTPS, set strong secrets, configure broker TLS and per-node ACLs, verify modem TLS support, add backups/rate limits/audit logging, and test with actual hardware. Haversine distances are approximate straight-line distances. Demo data is illustrative, not field measurements.
