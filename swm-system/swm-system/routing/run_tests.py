from test_routing import *
checks=[test_haversine_same_point,test_threshold,test_capacity_never_exceeded,test_unserved_when_vehicle_too_small]
for test in checks:
    test(); print('PASS',test.__name__)
print(f'{len(checks)} tests passed')
