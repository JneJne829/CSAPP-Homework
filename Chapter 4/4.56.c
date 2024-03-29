/* 
 * 
 * --- origin-pipe-btfnt.hcl	2021-02-25 07:26:33.309259378 +0000
 * +++ pipe-btfnt.hcl	2021-02-25 07:26:33.309259378 +0000
 * @@ -139,7 +139,10 @@
 * ## What address should instruction be fetched at
 * word f_pc = [
 * 	# Mispredicted branch.  Fetch at incremented PC
 * 	M_icode == IJXX && !M_Cnd : M_valA;
 * 	# backward taken error
 * 	M_icode == IJXX && M_ifun != UNCOND && M_valE < M_valA && !M_Cnd : M_valA;
 * 	# forward not-taken error
 * 	M_icode == IJXX && M_ifun != UNCOND && M_valE >= M_valA && M_Cnd : M_valE;
 * 	# Completion of RET instruction
 * 	W_icode == IRET : W_valM;
 * 	# Default: Use predicted value of PC
 * @@ -183,6 +186,8 @@
 * # Predict next value of PC
 * word f_predPC = [
 * 	# BBTFNT: This is where you'll change the branch prediction rule
 * 	f_icode == IJXX && f_ifun != UNCOND && f_valC < f_valP : f_valC;
 * 	f_icode == IJXX && f_ifun != UNCOND && f_valC >= f_valP : f_valP;
 * 	f_icode in { IJXX, ICALL } : f_valC;
 * 	1 : f_valP;
 * ];
 * @@ -244,12 +249,15 @@
 * # way to get valC into pipeline register M, so that
 * # you can correct for a mispredicted branch.
 * 
 * ## pass valC by M_valE, pass valP by M_valA
 * 
 * ## Select input A to ALU
 * word aluA = [
 * 	E_icode in { IRRMOVQ, IOPQ } : E_valA;
 * 	E_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ } : E_valC;
 * 	E_icode in { ICALL, IPUSHQ } : -8;
 * 	E_icode in { IRET, IPOPQ } : 8;
 * 	E_icode in { IJXX } : E_valC;
 * 	# Other instructions don't need ALU
 * ];
 * 
 * @@ -258,6 +266,7 @@
 * 	E_icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL, 
 * 		     IPUSHQ, IRET, IPOPQ } : E_valB;
 * 	E_icode in { IRRMOVQ, IIRMOVQ } : 0;
 * 	E_icode in { IJXX } : 0;
 * 	# Other instructions don't need ALU
 * ];
 * 
 * @@ -343,7 +352,11 @@
 * 
 * bool D_bubble =
 * 	# Mispredicted branch
 * 	(E_icode == IJXX && !e_Cnd) ||
 * 	# backward taken error or forward not-taken error
 * 	(
 * 	(E_icode == IJXX && E_ifun != UNCOND && E_valC < E_valA && !e_Cnd) ||
 * 	(E_icode == IJXX && E_ifun != UNCOND && E_valC >= E_valA && e_Cnd)
 * 	) ||
 * 	# BBTFNT: This condition will change
 * 	# Stalling at fetch while ret passes through pipeline
 * 	# but not condition for a load/use hazard
 * @@ -355,7 +368,11 @@
 * bool E_stall = 0;
 * bool E_bubble =
 * 	# Mispredicted branch
 * 	(E_icode == IJXX && !e_Cnd) ||
 * 	# backward taken error or forward not-taken error
 * 	(
 * 	(E_icode == IJXX && E_ifun != UNCOND && E_valC < E_valA && !e_Cnd) ||
 * 	(E_icode == IJXX && E_ifun != UNCOND && E_valC >= E_valA && e_Cnd)
 * 	) ||
 * 	# BBTFNT: This condition will change
 * 	# Conditions for a load/use hazard
 * 	E_icode in { IMRMOVQ, IPOPQ } &&
 * 
 */