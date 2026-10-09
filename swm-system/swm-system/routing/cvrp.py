"""Capacity-constrained nearest-neighbour CVRP heuristic + 2-opt; distances are Haversine estimates."""
import math
NOMINAL_BIN_KG=100.0
CRITICAL_PCT=80.0

def haversine(a,b):
    r=6371.0; la1,lo1,la2,lo2=map(math.radians,(a[0],a[1],b[0],b[1]))
    h=math.sin((la2-la1)/2)**2+math.cos(la1)*math.cos(la2)*math.sin((lo2-lo1)/2)**2
    return 2*r*math.asin(math.sqrt(min(1.0,h)))

def route_length(route,depot):
    pts=[depot]+[b['pos'] for b in route]+[depot]
    return sum(haversine(pts[i],pts[i+1]) for i in range(len(pts)-1))

def two_opt(route,depot):
    best=route[:]; improved=True
    while improved:
        improved=False
        for i in range(len(best)-1):
            for j in range(i+1,len(best)):
                candidate=best[:i]+best[i:j+1][::-1]+best[j+1:]
                if route_length(candidate,depot)<route_length(best,depot)-1e-9:
                    best=candidate; improved=True
    return best

def critical_bins(bins,threshold=CRITICAL_PCT,bin_kg=NOMINAL_BIN_KG):
    return [{'id':b['id'],'pos':tuple(b['pos']),'fill':float(b['fill']),'load':float(b['fill'])/100*bin_kg} for b in bins if float(b['fill'])>=threshold]

def solve_cvrp(bins,depot,vehicles):
    unserved=list(bins); plan=[]
    for vehicle in vehicles:
        vdepot=vehicle.get('depot',depot); route=[]; load=0.0; pos=vdepot
        while unserved:
            feasible=[b for b in unserved if load+b['load']<=vehicle['capacity']]
            if not feasible: break
            nxt=min(feasible,key=lambda b:haversine(pos,b['pos']))
            route.append(nxt);unserved.remove(nxt);load+=nxt['load'];pos=nxt['pos']
        if len(route)>2: route=two_opt(route,vdepot)
        plan.append({'vehicle':vehicle['id'],'stops':route,'load':load,'distance_km':route_length(route,vdepot)})
    return plan,[b['id'] for b in unserved]
