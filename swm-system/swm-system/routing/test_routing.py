from cvrp import haversine,critical_bins,solve_cvrp

def test_haversine_same_point(): assert haversine((-15,28),(-15,28))==0

def test_threshold():
    got=critical_bins([{'id':'a','pos':(0,0),'fill':79},{'id':'b','pos':(0,1),'fill':80}])
    assert [x['id'] for x in got]==['b']

def test_capacity_never_exceeded():
    bins=[{'id':f'b{i}','pos':(i*.001,0),'fill':80} for i in range(6)]
    plan,left=solve_cvrp(critical_bins(bins), (0,0), [{'id':'v1','capacity':170}])
    assert plan[0]['load']<=170
    assert len(plan[0]['stops'])+len(left)==6

def test_unserved_when_vehicle_too_small():
    bins=critical_bins([{'id':'b1','pos':(0,0),'fill':100}])
    plan,left=solve_cvrp(bins,(0,0),[{'id':'tiny','capacity':50}])
    assert left==['b1']
