/* 
 * 
 * --- origin-pipe-nt.hcl 2021-02-25 07:26:33.309259378 +0000
 * +++ pipe-nt.hcl 2021-02-25 07:26:33.309259378 +0000
 * @@ -139,7 +139,7 @@
 * ## What address should instruction be fetched at
 * word f_pc = [
 * 	# Mispredicted branch.  Fetch at incremented PC
 * -	M_icode == IJXX && !M_Cnd : M_valA;
 * +	M_icode == IJXX && M_ifun != UNCOND && M_Cnd : M_valA;
 * 	# Completion of RET instruction
 * 	W_icode == IRET : W_valM;
 * 	# Default: Use predicted value of PC
 * @@ -183,6 +183,7 @@
 * # Predict next value of PC
 * word f_predPC = [
 * 	# BNT: This is where you'll change the branch prediction rule
 * +	f_icode == IJXX && f_ifun != UNCOND : f_valP;
 * 	f_icode in { IJXX, ICALL } : f_valC;
 * 	1 : f_valP;
 * ];
 * @@ -273,7 +274,11 @@
 * 	!m_stat in { SADR, SINS, SHLT } && !W_stat in { SADR, SINS, SHLT };
 * 
 * ## Generate valA in execute stage
 *-word e_valA = E_valA;    # Pass valA through stage
 *+## pass branch address back by M_valA
 *+word e_valA = [
 *+	E_icode == IJXX && E_ifun != UNCOND : E_valC;
 *+	1 : E_valA;    # Pass valA through stage
 *+];
 * 
 * ## Set dstE to RNONE in event of not-taken conditional move
 * word e_dstE = [
 * @@ -343,7 +348,7 @@
 * 
 * bool D_bubble =
 * 	# Mispredicted branch
 *-	(E_icode == IJXX && !e_Cnd) ||
 *+	(E_icode == IJXX && E_ifun != UNCOND && e_Cnd) ||
 * 	# Stalling at fetch while ret passes through pipeline
 * 	# but not condition for a load/use hazard
 * 	!(E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB }) &&
 * @@ -354,7 +359,7 @@
 * bool E_stall = 0;
 * bool E_bubble =
 * 	# Mispredicted branch
 *-	(E_icode == IJXX && !e_Cnd) ||
 *+	(E_icode == IJXX && E_ifun != UNCOND && e_Cnd) ||
 * 	# Conditions for a load/use hazard
 * 	E_icode in { IMRMOVQ, IPOPQ } &&
 * 	 E_dstM in { d_srcA, d_srcB};
 * 
 */