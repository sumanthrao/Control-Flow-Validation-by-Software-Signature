# Control-Flow-Validation-by-Software-Signature
# Algorithm
G = global var </br> 
sig = map(bb:number) </br>
for each_bb in the function:</br>
  &nbsp;&nbsp;&nbsp;&nbsp;assign an unique value to each basic block and store it against the bb in a map </br>
 </br>
Control_flow_check():</br> 
  &nbsp;&nbsp;&nbsp;&nbsp;for each_bb in the function: </br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;if(it's not the beginning block): </br>
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;get the predecessor block as s_pre </br>
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;if s_cur is not branch-fan-in-node: </br>
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  calculate the d_cur value as s_cur ^ s_pre </br> 
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  Add an instruction in the basic block to calculate G_sig = d_cur ^ s_pre </br>
       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Add an instruction to check if (G_sig == S_cur) else raise an Excpetion </br>
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;else: </br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Handle_branch_fan_in(basicblock_ptr) </br>
       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; calculate the d_cur value as s_cur ^ s_chosen </br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Add an instruction in the basic block to calculate G_sig = d_cur ^ G_sig </br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Add an instruction in the basic block to calculate G_sig = G_sig ^ D </br>
        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Add an instruction to check if (G_sig == S_cur) else raise an Excpetion </br>
   
Handle_branch_fan_in(basicblock_ptr): </br>
  &nbsp;&nbsp;&nbsp;&nbsp;choose the first predecessor as s_chosen </br>
  &nbsp;&nbsp;&nbsp;&nbsp;for each predecessor of basic_block: </br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;calculate the D value as s_cur ^ s_chosen </br>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Add an instruction to store the D vakue in a global variable </br>
    </br>

Add_instruction(operand1, operand2, operation, target, bb_ptr): </br>
  &nbsp;&nbsp;&nbsp;&nbsp;add_the_instruction at the specified ptr </br>
    
    
  
