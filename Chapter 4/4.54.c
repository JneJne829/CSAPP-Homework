/* 
 * 
 * --- origin-pipe-full.hcl 2021-02-25 07:26:33.309259378 +0000
 * +++ pipe-full.hcl 2021-02-25 07:26:33.309259378 +0000
 * @@ -158,7 +158,7 @@
 * # Is instruction valid?
 * bool instr_valid = f_icode in 
 *     { INOP, IHALT, IRRMOVQ, IIRMOVQ, IRMMOVQ, IMRMOVQ,
 * -      IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ };
 * +      IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ, IIADDQ };
 * 
 * # Determine status code for fetched instruction
 * word f_stat = [
 * @@ -171,11 +171,11 @@
 * # Does fetched instruction require a regid byte?
 * bool need_regids =
 *     f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, 
 * -             IIRMOVQ, IRMMOVQ, IMRMOVQ };
 * +             IIRMOVQ, IRMMOVQ, IMRMOVQ, IIADDQ };
 * 
 * # Does fetched instruction require a constant word?
 * bool need_valC =
 * -    f_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL };
 * +    f_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL, IIADDQ };
 * 
 * # Predict next value of PC
 * word f_predPC = [
 * @@ -195,14 +195,14 @@
 * 
 * ## What register should be used as the B source?
 * word d_srcB = [
 * -    D_icode in { IOPQ, IRMMOVQ, IMRMOVQ  } : D_rB;
 * +    D_icode in { IOPQ, IRMMOVQ, IMRMOVQ, IIADDQ  } : D_rB;
 *     D_icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
 *     1 : RNONE;  # Don't need register
 * ];
 * 
 * ## What register should be used as the E destination?
 * word d_dstE = [
 * -    D_icode in { IRRMOVQ, IIRMOVQ, IOPQ} : D_rB;
 * +    D_icode in { IRRMOVQ, IIRMOVQ, IOPQ, IIADDQ } : D_rB;
 *     D_icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
 *     1 : RNONE;  # Don't write any register
 * ];
 * @@ -239,7 +239,7 @@
 * ## Select input A to ALU
 * word aluA = [
 *     E_icode in { IRRMOVQ, IOPQ } : E_valA;
 * -    E_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ } : E_valC;
 * +    E_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IIADDQ } : E_valC;
 *     E_icode in { ICALL, IPUSHQ } : -8;
 *     E_icode in { IRET, IPOPQ } : 8;
 *     # Other instructions don't need ALU
 * @@ -248,7 +248,7 @@
 * ## Select input B to ALU
 * word aluB = [
 *     E_icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL, 
 * -             IPUSHQ, IRET, IPOPQ } : E_valB;
 * +             IPUSHQ, IRET, IPOPQ, IIADDQ } : E_valB;
 *     E_icode in { IRRMOVQ, IIRMOVQ } : 0;
 *     # Other instructions don't need ALU
 * ];
 * @@ -260,7 +260,7 @@
 * ];
 * 
 * ## Should the condition codes be updated?
 * -bool set_cc = E_icode == IOPQ &&
 * +bool set_cc = (E_icode == IOPQ || E_icode == IIADDQ) &&
 *     # State changes only during normal operation
 *     !m_stat in { SADR, SINS, SHLT } && !W_stat in { SADR, SINS, SHLT };
 * 
 */