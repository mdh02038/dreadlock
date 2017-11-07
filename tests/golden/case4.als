abstract sig Direction {}
one sig IN, OUT extends Direction {}
abstract sig Bus {}
abstract sig Vc {}
abstract sig Boolean {}
abstract sig Unit {}
abstract sig Wire {
    unit: one Unit,
    bus: one Bus,
    vc: one Vc,
	 direction: one Direction,
    dep: set Wire,
    cdep: set Wire
}
pred deadlock_free {
    acyclic[dep]
}
// utility functions
pred irreflexive[rel: Wire->Wire]           { no iden & rel }
pred acyclic[rel: Wire->Wire]               { irreflexive[^rel] }
fact { all disj p1,p2: Wire | p1->p2 in cdep <=> p1->p2 in dep and  some p2.(^dep) :> p1 }
pred noconnect[ b: Bus, u: Unit ] {
    all v: Vc, d:  OUT | no dep[ u.~unit & b.~bus & v.~vc & d.~direction ]
}

pred connect[ b1: Bus, u1: Unit, b2: Bus, u2: Unit ] {
    all v: Vc | let p = u1.~unit & b1.~bus & v.~vc & OUT.~direction | some p => dep[p] = 
					u2.~unit & b2.~bus & v.~vc & IN.~direction
    all v: Vc | let p = u2.~unit & b2.~bus & v.~vc & OUT.~direction | some p => dep[p] = 
					u1.~unit & b1.~bus & v.~vc & IN.~direction
}

// Port signatures
one sig PCIE0, A0, PCIE1, A1, A2, A3, PCIE2, PCIE3, PCIE4 extends Bus {}

// Vc signatures
one sig REQ, RESP, VC0, VC1, POST, NPOST extends Vc {}

// Unit signatures
one sig case4, case4_PSWITCH0, case4_ASW0, case4_ADEV0, case4_ADEV1, case4_CDEV0, case4_PDEV0, case4_BDEV0, case4_BDEV1 extends Unit {}

// wire sigs for case4 

// wire sigs for case4_PSWITCH0 
one sig case4_PSWITCH0_PCIE0_POST_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE0 vc=POST direction=IN }
one sig case4_PSWITCH0_PCIE0_POST_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE0 vc=POST direction=OUT }
one sig case4_PSWITCH0_PCIE0_NPOST_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE0 vc=NPOST direction=IN }
one sig case4_PSWITCH0_PCIE0_NPOST_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE0 vc=NPOST direction=OUT }
one sig case4_PSWITCH0_PCIE0_RESP_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE0 vc=RESP direction=IN }
one sig case4_PSWITCH0_PCIE0_RESP_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE0 vc=RESP direction=OUT }
one sig case4_PSWITCH0_PCIE1_POST_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE1 vc=POST direction=IN }
one sig case4_PSWITCH0_PCIE1_POST_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE1 vc=POST direction=OUT }
one sig case4_PSWITCH0_PCIE1_NPOST_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE1 vc=NPOST direction=IN }
one sig case4_PSWITCH0_PCIE1_NPOST_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE1 vc=NPOST direction=OUT }
one sig case4_PSWITCH0_PCIE1_RESP_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE1 vc=RESP direction=IN }
one sig case4_PSWITCH0_PCIE1_RESP_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE1 vc=RESP direction=OUT }
one sig case4_PSWITCH0_PCIE2_POST_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE2 vc=POST direction=IN }
one sig case4_PSWITCH0_PCIE2_POST_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE2 vc=POST direction=OUT }
one sig case4_PSWITCH0_PCIE2_NPOST_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE2 vc=NPOST direction=IN }
one sig case4_PSWITCH0_PCIE2_NPOST_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE2 vc=NPOST direction=OUT }
one sig case4_PSWITCH0_PCIE2_RESP_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE2 vc=RESP direction=IN }
one sig case4_PSWITCH0_PCIE2_RESP_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE2 vc=RESP direction=OUT }
one sig case4_PSWITCH0_PCIE3_POST_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE3 vc=POST direction=IN }
one sig case4_PSWITCH0_PCIE3_POST_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE3 vc=POST direction=OUT }
one sig case4_PSWITCH0_PCIE3_NPOST_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE3 vc=NPOST direction=IN }
one sig case4_PSWITCH0_PCIE3_NPOST_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE3 vc=NPOST direction=OUT }
one sig case4_PSWITCH0_PCIE3_RESP_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE3 vc=RESP direction=IN }
one sig case4_PSWITCH0_PCIE3_RESP_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE3 vc=RESP direction=OUT }
one sig case4_PSWITCH0_PCIE4_POST_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE4 vc=POST direction=IN }
one sig case4_PSWITCH0_PCIE4_POST_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE4 vc=POST direction=OUT }
one sig case4_PSWITCH0_PCIE4_NPOST_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE4 vc=NPOST direction=IN }
one sig case4_PSWITCH0_PCIE4_NPOST_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE4 vc=NPOST direction=OUT }
one sig case4_PSWITCH0_PCIE4_RESP_IN extends Wire {} { unit=case4_PSWITCH0 bus=PCIE4 vc=RESP direction=IN }
one sig case4_PSWITCH0_PCIE4_RESP_OUT extends Wire {} { unit=case4_PSWITCH0 bus=PCIE4 vc=RESP direction=OUT }

// wire sigs for case4_ASW0 
one sig case4_ASW0_A0_REQ_IN extends Wire {} { unit=case4_ASW0 bus=A0 vc=REQ direction=IN }
one sig case4_ASW0_A0_REQ_OUT extends Wire {} { unit=case4_ASW0 bus=A0 vc=REQ direction=OUT }
one sig case4_ASW0_A0_RESP_IN extends Wire {} { unit=case4_ASW0 bus=A0 vc=RESP direction=IN }
one sig case4_ASW0_A0_RESP_OUT extends Wire {} { unit=case4_ASW0 bus=A0 vc=RESP direction=OUT }
one sig case4_ASW0_A0_VC0_IN extends Wire {} { unit=case4_ASW0 bus=A0 vc=VC0 direction=IN }
one sig case4_ASW0_A0_VC0_OUT extends Wire {} { unit=case4_ASW0 bus=A0 vc=VC0 direction=OUT }
one sig case4_ASW0_A0_VC1_IN extends Wire {} { unit=case4_ASW0 bus=A0 vc=VC1 direction=IN }
one sig case4_ASW0_A0_VC1_OUT extends Wire {} { unit=case4_ASW0 bus=A0 vc=VC1 direction=OUT }
one sig case4_ASW0_A1_REQ_IN extends Wire {} { unit=case4_ASW0 bus=A1 vc=REQ direction=IN }
one sig case4_ASW0_A1_REQ_OUT extends Wire {} { unit=case4_ASW0 bus=A1 vc=REQ direction=OUT }
one sig case4_ASW0_A1_RESP_IN extends Wire {} { unit=case4_ASW0 bus=A1 vc=RESP direction=IN }
one sig case4_ASW0_A1_RESP_OUT extends Wire {} { unit=case4_ASW0 bus=A1 vc=RESP direction=OUT }
one sig case4_ASW0_A1_VC0_IN extends Wire {} { unit=case4_ASW0 bus=A1 vc=VC0 direction=IN }
one sig case4_ASW0_A1_VC0_OUT extends Wire {} { unit=case4_ASW0 bus=A1 vc=VC0 direction=OUT }
one sig case4_ASW0_A1_VC1_IN extends Wire {} { unit=case4_ASW0 bus=A1 vc=VC1 direction=IN }
one sig case4_ASW0_A1_VC1_OUT extends Wire {} { unit=case4_ASW0 bus=A1 vc=VC1 direction=OUT }
one sig case4_ASW0_A2_REQ_IN extends Wire {} { unit=case4_ASW0 bus=A2 vc=REQ direction=IN }
one sig case4_ASW0_A2_REQ_OUT extends Wire {} { unit=case4_ASW0 bus=A2 vc=REQ direction=OUT }
one sig case4_ASW0_A2_RESP_IN extends Wire {} { unit=case4_ASW0 bus=A2 vc=RESP direction=IN }
one sig case4_ASW0_A2_RESP_OUT extends Wire {} { unit=case4_ASW0 bus=A2 vc=RESP direction=OUT }
one sig case4_ASW0_A2_VC0_IN extends Wire {} { unit=case4_ASW0 bus=A2 vc=VC0 direction=IN }
one sig case4_ASW0_A2_VC0_OUT extends Wire {} { unit=case4_ASW0 bus=A2 vc=VC0 direction=OUT }
one sig case4_ASW0_A2_VC1_IN extends Wire {} { unit=case4_ASW0 bus=A2 vc=VC1 direction=IN }
one sig case4_ASW0_A2_VC1_OUT extends Wire {} { unit=case4_ASW0 bus=A2 vc=VC1 direction=OUT }
one sig case4_ASW0_A3_REQ_IN extends Wire {} { unit=case4_ASW0 bus=A3 vc=REQ direction=IN }
one sig case4_ASW0_A3_REQ_OUT extends Wire {} { unit=case4_ASW0 bus=A3 vc=REQ direction=OUT }
one sig case4_ASW0_A3_RESP_IN extends Wire {} { unit=case4_ASW0 bus=A3 vc=RESP direction=IN }
one sig case4_ASW0_A3_RESP_OUT extends Wire {} { unit=case4_ASW0 bus=A3 vc=RESP direction=OUT }
one sig case4_ASW0_A3_VC0_IN extends Wire {} { unit=case4_ASW0 bus=A3 vc=VC0 direction=IN }
one sig case4_ASW0_A3_VC0_OUT extends Wire {} { unit=case4_ASW0 bus=A3 vc=VC0 direction=OUT }
one sig case4_ASW0_A3_VC1_IN extends Wire {} { unit=case4_ASW0 bus=A3 vc=VC1 direction=IN }
one sig case4_ASW0_A3_VC1_OUT extends Wire {} { unit=case4_ASW0 bus=A3 vc=VC1 direction=OUT }

// wire sigs for case4_ADEV0 
// one sig case4_ADEV0_PCIE0_POST_IN extends Wire {} { unit=case4_ADEV0 bus=PCIE0 vc=POST direction=IN }
one sig case4_ADEV0_PCIE0_POST_OUT extends Wire {} { unit=case4_ADEV0 bus=PCIE0 vc=POST direction=OUT }
one sig case4_ADEV0_PCIE0_NPOST_IN extends Wire {} { unit=case4_ADEV0 bus=PCIE0 vc=NPOST direction=IN }
one sig case4_ADEV0_PCIE0_NPOST_OUT extends Wire {} { unit=case4_ADEV0 bus=PCIE0 vc=NPOST direction=OUT }
// one sig case4_ADEV0_PCIE0_RESP_IN extends Wire {} { unit=case4_ADEV0 bus=PCIE0 vc=RESP direction=IN }
one sig case4_ADEV0_PCIE0_RESP_OUT extends Wire {} { unit=case4_ADEV0 bus=PCIE0 vc=RESP direction=OUT }
one sig case4_ADEV0_A0_REQ_IN extends Wire {} { unit=case4_ADEV0 bus=A0 vc=REQ direction=IN }
one sig case4_ADEV0_A0_REQ_OUT extends Wire {} { unit=case4_ADEV0 bus=A0 vc=REQ direction=OUT }
// one sig case4_ADEV0_A0_RESP_IN extends Wire {} { unit=case4_ADEV0 bus=A0 vc=RESP direction=IN }
one sig case4_ADEV0_A0_RESP_OUT extends Wire {} { unit=case4_ADEV0 bus=A0 vc=RESP direction=OUT }
one sig case4_ADEV0_A0_VC0_IN extends Wire {} { unit=case4_ADEV0 bus=A0 vc=VC0 direction=IN }
// one sig case4_ADEV0_A0_VC0_OUT extends Wire {} { unit=case4_ADEV0 bus=A0 vc=VC0 direction=OUT }
// one sig case4_ADEV0_A0_VC1_IN extends Wire {} { unit=case4_ADEV0 bus=A0 vc=VC1 direction=IN }
one sig case4_ADEV0_A0_VC1_OUT extends Wire {} { unit=case4_ADEV0 bus=A0 vc=VC1 direction=OUT }

// wire sigs for case4_ADEV1 
// one sig case4_ADEV1_PCIE0_POST_IN extends Wire {} { unit=case4_ADEV1 bus=PCIE0 vc=POST direction=IN }
one sig case4_ADEV1_PCIE0_POST_OUT extends Wire {} { unit=case4_ADEV1 bus=PCIE0 vc=POST direction=OUT }
one sig case4_ADEV1_PCIE0_NPOST_IN extends Wire {} { unit=case4_ADEV1 bus=PCIE0 vc=NPOST direction=IN }
one sig case4_ADEV1_PCIE0_NPOST_OUT extends Wire {} { unit=case4_ADEV1 bus=PCIE0 vc=NPOST direction=OUT }
// one sig case4_ADEV1_PCIE0_RESP_IN extends Wire {} { unit=case4_ADEV1 bus=PCIE0 vc=RESP direction=IN }
one sig case4_ADEV1_PCIE0_RESP_OUT extends Wire {} { unit=case4_ADEV1 bus=PCIE0 vc=RESP direction=OUT }
one sig case4_ADEV1_A0_REQ_IN extends Wire {} { unit=case4_ADEV1 bus=A0 vc=REQ direction=IN }
one sig case4_ADEV1_A0_REQ_OUT extends Wire {} { unit=case4_ADEV1 bus=A0 vc=REQ direction=OUT }
// one sig case4_ADEV1_A0_RESP_IN extends Wire {} { unit=case4_ADEV1 bus=A0 vc=RESP direction=IN }
one sig case4_ADEV1_A0_RESP_OUT extends Wire {} { unit=case4_ADEV1 bus=A0 vc=RESP direction=OUT }
one sig case4_ADEV1_A0_VC0_IN extends Wire {} { unit=case4_ADEV1 bus=A0 vc=VC0 direction=IN }
// one sig case4_ADEV1_A0_VC0_OUT extends Wire {} { unit=case4_ADEV1 bus=A0 vc=VC0 direction=OUT }
// one sig case4_ADEV1_A0_VC1_IN extends Wire {} { unit=case4_ADEV1 bus=A0 vc=VC1 direction=IN }
one sig case4_ADEV1_A0_VC1_OUT extends Wire {} { unit=case4_ADEV1 bus=A0 vc=VC1 direction=OUT }

// wire sigs for case4_CDEV0 
one sig case4_CDEV0_PCIE0_POST_IN extends Wire {} { unit=case4_CDEV0 bus=PCIE0 vc=POST direction=IN }
one sig case4_CDEV0_PCIE0_POST_OUT extends Wire {} { unit=case4_CDEV0 bus=PCIE0 vc=POST direction=OUT }
one sig case4_CDEV0_PCIE0_NPOST_IN extends Wire {} { unit=case4_CDEV0 bus=PCIE0 vc=NPOST direction=IN }
one sig case4_CDEV0_PCIE0_NPOST_OUT extends Wire {} { unit=case4_CDEV0 bus=PCIE0 vc=NPOST direction=OUT }
one sig case4_CDEV0_PCIE0_RESP_IN extends Wire {} { unit=case4_CDEV0 bus=PCIE0 vc=RESP direction=IN }
one sig case4_CDEV0_PCIE0_RESP_OUT extends Wire {} { unit=case4_CDEV0 bus=PCIE0 vc=RESP direction=OUT }
one sig case4_CDEV0_A0_REQ_IN extends Wire {} { unit=case4_CDEV0 bus=A0 vc=REQ direction=IN }
one sig case4_CDEV0_A0_REQ_OUT extends Wire {} { unit=case4_CDEV0 bus=A0 vc=REQ direction=OUT }
// one sig case4_CDEV0_A0_RESP_IN extends Wire {} { unit=case4_CDEV0 bus=A0 vc=RESP direction=IN }
one sig case4_CDEV0_A0_RESP_OUT extends Wire {} { unit=case4_CDEV0 bus=A0 vc=RESP direction=OUT }
// one sig case4_CDEV0_A0_VC0_IN extends Wire {} { unit=case4_CDEV0 bus=A0 vc=VC0 direction=IN }
// one sig case4_CDEV0_A0_VC0_OUT extends Wire {} { unit=case4_CDEV0 bus=A0 vc=VC0 direction=OUT }
one sig case4_CDEV0_A0_VC1_IN extends Wire {} { unit=case4_CDEV0 bus=A0 vc=VC1 direction=IN }
// one sig case4_CDEV0_A0_VC1_OUT extends Wire {} { unit=case4_CDEV0 bus=A0 vc=VC1 direction=OUT }

// wire sigs for case4_PDEV0 
// one sig case4_PDEV0_PCIE0_POST_IN extends Wire {} { unit=case4_PDEV0 bus=PCIE0 vc=POST direction=IN }
// one sig case4_PDEV0_PCIE0_POST_OUT extends Wire {} { unit=case4_PDEV0 bus=PCIE0 vc=POST direction=OUT }
one sig case4_PDEV0_PCIE0_NPOST_IN extends Wire {} { unit=case4_PDEV0 bus=PCIE0 vc=NPOST direction=IN }
// one sig case4_PDEV0_PCIE0_NPOST_OUT extends Wire {} { unit=case4_PDEV0 bus=PCIE0 vc=NPOST direction=OUT }
// one sig case4_PDEV0_PCIE0_RESP_IN extends Wire {} { unit=case4_PDEV0 bus=PCIE0 vc=RESP direction=IN }
one sig case4_PDEV0_PCIE0_RESP_OUT extends Wire {} { unit=case4_PDEV0 bus=PCIE0 vc=RESP direction=OUT }

// wire sigs for case4_BDEV0 
one sig case4_BDEV0_PCIE0_POST_IN extends Wire {} { unit=case4_BDEV0 bus=PCIE0 vc=POST direction=IN }
one sig case4_BDEV0_PCIE0_POST_OUT extends Wire {} { unit=case4_BDEV0 bus=PCIE0 vc=POST direction=OUT }
one sig case4_BDEV0_PCIE0_NPOST_IN extends Wire {} { unit=case4_BDEV0 bus=PCIE0 vc=NPOST direction=IN }
one sig case4_BDEV0_PCIE0_NPOST_OUT extends Wire {} { unit=case4_BDEV0 bus=PCIE0 vc=NPOST direction=OUT }
one sig case4_BDEV0_PCIE0_RESP_IN extends Wire {} { unit=case4_BDEV0 bus=PCIE0 vc=RESP direction=IN }
one sig case4_BDEV0_PCIE0_RESP_OUT extends Wire {} { unit=case4_BDEV0 bus=PCIE0 vc=RESP direction=OUT }
one sig case4_BDEV0_PCIE1_POST_IN extends Wire {} { unit=case4_BDEV0 bus=PCIE1 vc=POST direction=IN }
one sig case4_BDEV0_PCIE1_POST_OUT extends Wire {} { unit=case4_BDEV0 bus=PCIE1 vc=POST direction=OUT }
one sig case4_BDEV0_PCIE1_NPOST_IN extends Wire {} { unit=case4_BDEV0 bus=PCIE1 vc=NPOST direction=IN }
one sig case4_BDEV0_PCIE1_NPOST_OUT extends Wire {} { unit=case4_BDEV0 bus=PCIE1 vc=NPOST direction=OUT }
one sig case4_BDEV0_PCIE1_RESP_IN extends Wire {} { unit=case4_BDEV0 bus=PCIE1 vc=RESP direction=IN }
one sig case4_BDEV0_PCIE1_RESP_OUT extends Wire {} { unit=case4_BDEV0 bus=PCIE1 vc=RESP direction=OUT }
one sig case4_BDEV0_A0_REQ_IN extends Wire {} { unit=case4_BDEV0 bus=A0 vc=REQ direction=IN }
one sig case4_BDEV0_A0_REQ_OUT extends Wire {} { unit=case4_BDEV0 bus=A0 vc=REQ direction=OUT }
// one sig case4_BDEV0_A0_RESP_IN extends Wire {} { unit=case4_BDEV0 bus=A0 vc=RESP direction=IN }
one sig case4_BDEV0_A0_RESP_OUT extends Wire {} { unit=case4_BDEV0 bus=A0 vc=RESP direction=OUT }
// one sig case4_BDEV0_A0_VC0_IN extends Wire {} { unit=case4_BDEV0 bus=A0 vc=VC0 direction=IN }
// one sig case4_BDEV0_A0_VC0_OUT extends Wire {} { unit=case4_BDEV0 bus=A0 vc=VC0 direction=OUT }
// one sig case4_BDEV0_A0_VC1_IN extends Wire {} { unit=case4_BDEV0 bus=A0 vc=VC1 direction=IN }
// one sig case4_BDEV0_A0_VC1_OUT extends Wire {} { unit=case4_BDEV0 bus=A0 vc=VC1 direction=OUT }

// wire sigs for case4_BDEV1 
one sig case4_BDEV1_PCIE0_POST_IN extends Wire {} { unit=case4_BDEV1 bus=PCIE0 vc=POST direction=IN }
one sig case4_BDEV1_PCIE0_POST_OUT extends Wire {} { unit=case4_BDEV1 bus=PCIE0 vc=POST direction=OUT }
one sig case4_BDEV1_PCIE0_NPOST_IN extends Wire {} { unit=case4_BDEV1 bus=PCIE0 vc=NPOST direction=IN }
one sig case4_BDEV1_PCIE0_NPOST_OUT extends Wire {} { unit=case4_BDEV1 bus=PCIE0 vc=NPOST direction=OUT }
one sig case4_BDEV1_PCIE0_RESP_IN extends Wire {} { unit=case4_BDEV1 bus=PCIE0 vc=RESP direction=IN }
one sig case4_BDEV1_PCIE0_RESP_OUT extends Wire {} { unit=case4_BDEV1 bus=PCIE0 vc=RESP direction=OUT }
one sig case4_BDEV1_PCIE1_POST_IN extends Wire {} { unit=case4_BDEV1 bus=PCIE1 vc=POST direction=IN }
one sig case4_BDEV1_PCIE1_POST_OUT extends Wire {} { unit=case4_BDEV1 bus=PCIE1 vc=POST direction=OUT }
one sig case4_BDEV1_PCIE1_NPOST_IN extends Wire {} { unit=case4_BDEV1 bus=PCIE1 vc=NPOST direction=IN }
one sig case4_BDEV1_PCIE1_NPOST_OUT extends Wire {} { unit=case4_BDEV1 bus=PCIE1 vc=NPOST direction=OUT }
one sig case4_BDEV1_PCIE1_RESP_IN extends Wire {} { unit=case4_BDEV1 bus=PCIE1 vc=RESP direction=IN }
one sig case4_BDEV1_PCIE1_RESP_OUT extends Wire {} { unit=case4_BDEV1 bus=PCIE1 vc=RESP direction=OUT }
one sig case4_BDEV1_A0_REQ_IN extends Wire {} { unit=case4_BDEV1 bus=A0 vc=REQ direction=IN }
one sig case4_BDEV1_A0_REQ_OUT extends Wire {} { unit=case4_BDEV1 bus=A0 vc=REQ direction=OUT }
one sig case4_BDEV1_A0_RESP_IN extends Wire {} { unit=case4_BDEV1 bus=A0 vc=RESP direction=IN }
one sig case4_BDEV1_A0_RESP_OUT extends Wire {} { unit=case4_BDEV1 bus=A0 vc=RESP direction=OUT }
// one sig case4_BDEV1_A0_VC0_IN extends Wire {} { unit=case4_BDEV1 bus=A0 vc=VC0 direction=IN }
// one sig case4_BDEV1_A0_VC0_OUT extends Wire {} { unit=case4_BDEV1 bus=A0 vc=VC0 direction=OUT }
// one sig case4_BDEV1_A0_VC1_IN extends Wire {} { unit=case4_BDEV1 bus=A0 vc=VC1 direction=IN }
// one sig case4_BDEV1_A0_VC1_OUT extends Wire {} { unit=case4_BDEV1 bus=A0 vc=VC1 direction=OUT }


// bus connection rules// A connection rules
pred A_noconnect[ b: Bus, u: Unit ] {
    all v: REQ, d:  OUT | no dep[ u.~unit & b.~bus & v.~vc & d.~direction ]
    all v: RESP, d:  OUT | no dep[ u.~unit & b.~bus & v.~vc & d.~direction ]
    all v: VC0, d:  OUT | no dep[ u.~unit & b.~bus & v.~vc & d.~direction ]
    all v: VC1, d:  OUT | no dep[ u.~unit & b.~bus & v.~vc & d.~direction ]
}


pred A_connect[ b1: Bus, u1: Unit, b2: Bus, u2: Unit ] {
    all v1: REQ | let p = u1.~unit & b1.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
				u2.~unit & b2.~bus & (REQ).~vc & IN.~direction
    all v1: REQ | let p = u2.~unit & b2.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
					u1.~unit & b1.~bus & (REQ).~vc & IN.~direction
    all v1: RESP | let p = u1.~unit & b1.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
				u2.~unit & b2.~bus & (RESP).~vc & IN.~direction
    all v1: RESP | let p = u2.~unit & b2.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
					u1.~unit & b1.~bus & (RESP).~vc & IN.~direction
    all v1: VC0 | let p = u1.~unit & b1.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
				u2.~unit & b2.~bus & (VC0).~vc & IN.~direction
    all v1: VC0 | let p = u2.~unit & b2.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
					u1.~unit & b1.~bus & (VC0).~vc & IN.~direction
    all v1: VC1 | let p = u1.~unit & b1.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
				u2.~unit & b2.~bus & (VC1).~vc & IN.~direction
    all v1: VC1 | let p = u2.~unit & b2.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
					u1.~unit & b1.~bus & (VC1).~vc & IN.~direction
}
// PCIE connection rules
pred PCIE_noconnect[ b: Bus, u: Unit ] {
    all v: POST, d:  OUT | no dep[ u.~unit & b.~bus & v.~vc & d.~direction ]
    all v: NPOST, d:  OUT | no dep[ u.~unit & b.~bus & v.~vc & d.~direction ]
    all v: RESP, d:  OUT | no dep[ u.~unit & b.~bus & v.~vc & d.~direction ]
}


pred PCIE_connect[ b1: Bus, u1: Unit, b2: Bus, u2: Unit ] {
    all v1: POST | let p = u1.~unit & b1.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
				u2.~unit & b2.~bus & (POST).~vc & IN.~direction
    all v1: POST | let p = u2.~unit & b2.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
					u1.~unit & b1.~bus & (POST).~vc & IN.~direction
    all v1: NPOST | let p = u1.~unit & b1.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
				u2.~unit & b2.~bus & (RESP + POST + NPOST).~vc & IN.~direction
    all v1: NPOST | let p = u2.~unit & b2.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
					u1.~unit & b1.~bus & (RESP + POST + NPOST).~vc & IN.~direction
    all v1: RESP | let p = u1.~unit & b1.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
				u2.~unit & b2.~bus & (RESP + POST).~vc & IN.~direction
    all v1: RESP | let p = u2.~unit & b2.~bus & v1.~vc & OUT.~direction | some p => dep[p] = 
					u1.~unit & b1.~bus & (RESP + POST).~vc & IN.~direction
}

// instance rules

// case4 rules
fact {
}

// case4_PSWITCH0 rules
fact {
    dep[case4_PSWITCH0_PCIE0_NPOST_IN] =  case4_PSWITCH0_PCIE0_RESP_OUT + case4_PSWITCH0_PCIE1_NPOST_OUT + case4_PSWITCH0_PCIE2_NPOST_OUT + case4_PSWITCH0_PCIE3_NPOST_OUT + case4_PSWITCH0_PCIE4_NPOST_OUT
    dep[case4_PSWITCH0_PCIE0_POST_IN] =  case4_PSWITCH0_PCIE1_POST_OUT + case4_PSWITCH0_PCIE2_POST_OUT + case4_PSWITCH0_PCIE3_POST_OUT + case4_PSWITCH0_PCIE4_POST_OUT
    dep[case4_PSWITCH0_PCIE0_RESP_IN] =  case4_PSWITCH0_PCIE1_RESP_OUT + case4_PSWITCH0_PCIE2_RESP_OUT + case4_PSWITCH0_PCIE3_RESP_OUT + case4_PSWITCH0_PCIE4_RESP_OUT
    dep[case4_PSWITCH0_PCIE1_NPOST_IN] =  case4_PSWITCH0_PCIE0_NPOST_OUT + case4_PSWITCH0_PCIE1_RESP_OUT + case4_PSWITCH0_PCIE2_NPOST_OUT + case4_PSWITCH0_PCIE3_NPOST_OUT + case4_PSWITCH0_PCIE4_NPOST_OUT
    dep[case4_PSWITCH0_PCIE1_POST_IN] =  case4_PSWITCH0_PCIE0_POST_OUT + case4_PSWITCH0_PCIE2_POST_OUT + case4_PSWITCH0_PCIE3_POST_OUT + case4_PSWITCH0_PCIE4_POST_OUT
    dep[case4_PSWITCH0_PCIE1_RESP_IN] =  case4_PSWITCH0_PCIE0_RESP_OUT + case4_PSWITCH0_PCIE2_RESP_OUT + case4_PSWITCH0_PCIE3_RESP_OUT + case4_PSWITCH0_PCIE4_RESP_OUT
    dep[case4_PSWITCH0_PCIE2_NPOST_IN] =  case4_PSWITCH0_PCIE0_NPOST_OUT + case4_PSWITCH0_PCIE1_NPOST_OUT + case4_PSWITCH0_PCIE2_RESP_OUT + case4_PSWITCH0_PCIE3_NPOST_OUT + case4_PSWITCH0_PCIE4_NPOST_OUT
    dep[case4_PSWITCH0_PCIE2_POST_IN] =  case4_PSWITCH0_PCIE0_POST_OUT + case4_PSWITCH0_PCIE1_POST_OUT + case4_PSWITCH0_PCIE3_POST_OUT + case4_PSWITCH0_PCIE4_POST_OUT
    dep[case4_PSWITCH0_PCIE2_RESP_IN] =  case4_PSWITCH0_PCIE0_RESP_OUT + case4_PSWITCH0_PCIE1_RESP_OUT + case4_PSWITCH0_PCIE3_RESP_OUT + case4_PSWITCH0_PCIE4_RESP_OUT
    dep[case4_PSWITCH0_PCIE3_NPOST_IN] =  case4_PSWITCH0_PCIE0_NPOST_OUT + case4_PSWITCH0_PCIE1_NPOST_OUT + case4_PSWITCH0_PCIE2_NPOST_OUT + case4_PSWITCH0_PCIE3_RESP_OUT + case4_PSWITCH0_PCIE4_NPOST_OUT
    dep[case4_PSWITCH0_PCIE3_POST_IN] =  case4_PSWITCH0_PCIE0_POST_OUT + case4_PSWITCH0_PCIE1_POST_OUT + case4_PSWITCH0_PCIE2_POST_OUT + case4_PSWITCH0_PCIE4_POST_OUT
    dep[case4_PSWITCH0_PCIE3_RESP_IN] =  case4_PSWITCH0_PCIE0_RESP_OUT + case4_PSWITCH0_PCIE1_RESP_OUT + case4_PSWITCH0_PCIE2_RESP_OUT + case4_PSWITCH0_PCIE4_RESP_OUT
    dep[case4_PSWITCH0_PCIE4_NPOST_IN] =  case4_PSWITCH0_PCIE0_NPOST_OUT + case4_PSWITCH0_PCIE1_NPOST_OUT + case4_PSWITCH0_PCIE2_NPOST_OUT + case4_PSWITCH0_PCIE3_NPOST_OUT + case4_PSWITCH0_PCIE4_RESP_OUT
    dep[case4_PSWITCH0_PCIE4_POST_IN] =  case4_PSWITCH0_PCIE0_POST_OUT + case4_PSWITCH0_PCIE1_POST_OUT + case4_PSWITCH0_PCIE2_POST_OUT + case4_PSWITCH0_PCIE3_POST_OUT
    dep[case4_PSWITCH0_PCIE4_RESP_IN] =  case4_PSWITCH0_PCIE0_RESP_OUT + case4_PSWITCH0_PCIE1_RESP_OUT + case4_PSWITCH0_PCIE2_RESP_OUT + case4_PSWITCH0_PCIE3_RESP_OUT
}

// case4_ASW0 rules
fact {
    dep[case4_ASW0_A0_REQ_IN] =  case4_ASW0_A1_REQ_OUT + case4_ASW0_A2_REQ_OUT + case4_ASW0_A3_REQ_OUT
    dep[case4_ASW0_A0_RESP_IN] =  case4_ASW0_A1_RESP_OUT + case4_ASW0_A2_RESP_OUT + case4_ASW0_A3_RESP_OUT
    dep[case4_ASW0_A0_VC0_IN] =  case4_ASW0_A1_VC0_OUT + case4_ASW0_A2_VC0_OUT + case4_ASW0_A3_VC0_OUT
    dep[case4_ASW0_A0_VC1_IN] =  case4_ASW0_A1_VC1_OUT + case4_ASW0_A2_VC1_OUT + case4_ASW0_A3_VC1_OUT
    dep[case4_ASW0_A1_REQ_IN] =  case4_ASW0_A0_REQ_OUT + case4_ASW0_A2_REQ_OUT + case4_ASW0_A3_REQ_OUT
    dep[case4_ASW0_A1_RESP_IN] =  case4_ASW0_A0_RESP_OUT + case4_ASW0_A2_RESP_OUT + case4_ASW0_A3_RESP_OUT
    dep[case4_ASW0_A1_VC0_IN] =  case4_ASW0_A0_VC0_OUT + case4_ASW0_A2_VC0_OUT + case4_ASW0_A3_VC0_OUT
    dep[case4_ASW0_A1_VC1_IN] =  case4_ASW0_A0_VC1_OUT + case4_ASW0_A2_VC1_OUT + case4_ASW0_A3_VC1_OUT
    dep[case4_ASW0_A2_REQ_IN] =  case4_ASW0_A0_REQ_OUT + case4_ASW0_A1_REQ_OUT
    dep[case4_ASW0_A2_RESP_IN] =  case4_ASW0_A0_RESP_OUT + case4_ASW0_A1_RESP_OUT
    dep[case4_ASW0_A2_VC0_IN] =  case4_ASW0_A0_VC0_OUT + case4_ASW0_A1_VC0_OUT
    dep[case4_ASW0_A2_VC1_IN] =  case4_ASW0_A0_VC1_OUT + case4_ASW0_A1_VC1_OUT
    dep[case4_ASW0_A3_REQ_IN] =  case4_ASW0_A0_REQ_OUT + case4_ASW0_A1_REQ_OUT
    dep[case4_ASW0_A3_RESP_IN] =  case4_ASW0_A0_RESP_OUT + case4_ASW0_A1_RESP_OUT
    dep[case4_ASW0_A3_VC0_IN] =  case4_ASW0_A0_VC0_OUT + case4_ASW0_A1_VC0_OUT
    dep[case4_ASW0_A3_VC1_IN] =  case4_ASW0_A0_VC1_OUT + case4_ASW0_A1_VC1_OUT
}

// case4_ADEV0 rules
fact {
    dep[case4_ADEV0_A0_REQ_IN] =  case4_ADEV0_A0_RESP_OUT + case4_ADEV0_A0_VC1_OUT
//    no dep[case4_ADEV0_A0_RESP_IN]
    dep[case4_ADEV0_A0_VC0_IN] =  case4_ADEV0_A0_REQ_OUT + case4_ADEV0_A0_RESP_OUT + case4_ADEV0_A0_VC1_OUT + case4_ADEV0_PCIE0_NPOST_OUT + case4_ADEV0_PCIE0_POST_OUT
//    no dep[case4_ADEV0_A0_VC1_IN]
    dep[case4_ADEV0_PCIE0_NPOST_IN] =  case4_ADEV0_PCIE0_RESP_OUT
//    no dep[case4_ADEV0_PCIE0_POST_IN]
//    no dep[case4_ADEV0_PCIE0_RESP_IN]
}

// case4_ADEV1 rules
fact {
    dep[case4_ADEV1_A0_REQ_IN] =  case4_ADEV1_A0_RESP_OUT + case4_ADEV1_A0_VC1_OUT
//    no dep[case4_ADEV1_A0_RESP_IN]
    dep[case4_ADEV1_A0_VC0_IN] =  case4_ADEV1_A0_REQ_OUT + case4_ADEV1_A0_RESP_OUT + case4_ADEV1_A0_VC1_OUT + case4_ADEV1_PCIE0_NPOST_OUT + case4_ADEV1_PCIE0_POST_OUT
//    no dep[case4_ADEV1_A0_VC1_IN]
    dep[case4_ADEV1_PCIE0_NPOST_IN] =  case4_ADEV1_PCIE0_RESP_OUT
//    no dep[case4_ADEV1_PCIE0_POST_IN]
//    no dep[case4_ADEV1_PCIE0_RESP_IN]
}

// case4_CDEV0 rules
fact {
    dep[case4_CDEV0_A0_REQ_IN] =  case4_CDEV0_A0_REQ_OUT + case4_CDEV0_A0_RESP_OUT
//    no dep[case4_CDEV0_A0_RESP_IN]
//    no dep[case4_CDEV0_A0_VC0_IN]
    dep[case4_CDEV0_A0_VC1_IN] =  case4_CDEV0_A0_RESP_OUT
    dep[case4_CDEV0_PCIE0_NPOST_IN] =  case4_CDEV0_A0_REQ_OUT + case4_CDEV0_PCIE0_NPOST_OUT + case4_CDEV0_PCIE0_RESP_OUT
    dep[case4_CDEV0_PCIE0_POST_IN] =  case4_CDEV0_A0_REQ_OUT + case4_CDEV0_PCIE0_POST_OUT
    dep[case4_CDEV0_PCIE0_RESP_IN] =  case4_CDEV0_PCIE0_RESP_OUT
}

// case4_PDEV0 rules
fact {
    dep[case4_PDEV0_PCIE0_NPOST_IN] =  case4_PDEV0_PCIE0_RESP_OUT
//    no dep[case4_PDEV0_PCIE0_POST_IN]
//    no dep[case4_PDEV0_PCIE0_RESP_IN]
}

// case4_BDEV0 rules
fact {
    dep[case4_BDEV0_A0_REQ_IN] =  case4_BDEV0_A0_RESP_OUT + case4_BDEV0_PCIE0_NPOST_OUT + case4_BDEV0_PCIE0_POST_OUT
//    no dep[case4_BDEV0_A0_RESP_IN]
//    no dep[case4_BDEV0_A0_VC0_IN]
//    no dep[case4_BDEV0_A0_VC1_IN]
    dep[case4_BDEV0_PCIE0_NPOST_IN] =  case4_BDEV0_PCIE0_RESP_OUT + case4_BDEV0_PCIE1_NPOST_OUT
    dep[case4_BDEV0_PCIE0_POST_IN] =  case4_BDEV0_A0_REQ_OUT + case4_BDEV0_PCIE1_POST_OUT
    dep[case4_BDEV0_PCIE0_RESP_IN] =  case4_BDEV0_PCIE1_RESP_OUT
    dep[case4_BDEV0_PCIE1_NPOST_IN] =  case4_BDEV0_PCIE0_NPOST_OUT
    dep[case4_BDEV0_PCIE1_POST_IN] =  case4_BDEV0_PCIE0_POST_OUT
    dep[case4_BDEV0_PCIE1_RESP_IN] =  case4_BDEV0_PCIE0_RESP_OUT
}

// case4_BDEV1 rules
fact {
    dep[case4_BDEV1_A0_REQ_IN] =  case4_BDEV1_A0_RESP_OUT + case4_BDEV1_PCIE0_POST_OUT
    dep[case4_BDEV1_A0_RESP_IN] =  case4_BDEV1_PCIE0_RESP_OUT
//    no dep[case4_BDEV1_A0_VC0_IN]
//    no dep[case4_BDEV1_A0_VC1_IN]
    dep[case4_BDEV1_PCIE0_NPOST_IN] =  case4_BDEV1_A0_REQ_OUT + case4_BDEV1_PCIE1_NPOST_OUT
    dep[case4_BDEV1_PCIE0_POST_IN] =  case4_BDEV1_A0_REQ_OUT + case4_BDEV1_PCIE1_POST_OUT
    dep[case4_BDEV1_PCIE0_RESP_IN] =  case4_BDEV1_PCIE1_RESP_OUT
    dep[case4_BDEV1_PCIE1_NPOST_IN] =  case4_BDEV1_PCIE0_NPOST_OUT + case4_BDEV1_PCIE1_RESP_OUT
    dep[case4_BDEV1_PCIE1_POST_IN] =  case4_BDEV1_PCIE0_POST_OUT
    dep[case4_BDEV1_PCIE1_RESP_IN] =  case4_BDEV1_PCIE0_RESP_OUT
}

// connections for case4 {
pred case4 {
	PCIE_connect[PCIE0, case4_PSWITCH0, PCIE0, case4_CDEV0]
	PCIE_connect[PCIE1, case4_PSWITCH0, PCIE0, case4_ADEV0]
	PCIE_connect[PCIE2, case4_PSWITCH0, PCIE0, case4_ADEV1]
	A_connect[A0, case4_ASW0, A0, case4_ADEV0]
	A_connect[A1, case4_ASW0, A0, case4_ADEV1]
	PCIE_connect[PCIE3, case4_PSWITCH0, PCIE0, case4_BDEV0]
	A_connect[A2, case4_ASW0, A0, case4_BDEV0]
	PCIE_connect[PCIE4, case4_PSWITCH0, PCIE1, case4_BDEV1]
	PCIE_connect[PCIE0, case4_PDEV0, PCIE0, case4_BDEV1]
	A_connect[A3, case4_ASW0, A0, case4_BDEV1]
	A_noconnect[A0, case4_CDEV0]
	PCIE_noconnect[PCIE1, case4_BDEV0]
}

// Check statements
check case4 { case4 => deadlock_free }

// Run statements
run case4 { case4 }
