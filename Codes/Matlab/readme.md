The files *Cascade_Control_CRP_Simulink* consists of a simulator of the CRP, controlled via a cascade control. 
It can serve as simulator for testing other controller.

Note: the simulator must be used with the following MATLAB files:
  - *Cascade_Control_CRP.m*: design of the controller and assignement of parameters values
  - *physical_constraint.m* and *static_friction.m*: matlab function required to simulate the CRP physics
