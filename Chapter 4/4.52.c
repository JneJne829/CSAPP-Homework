/* 
 * 
 * --- origin-seq-full.hcl    2021-02-25 07:26:33.312592690 +0000
 * +++ seq-full.hcl    2021-02-25 07:26:33.312592690 +0000
 * @@ -107,16 +107,16 @@
 * 
 * bool instr_valid = icode in
 *     { INOP, IHALT, IRRMOVQ, IIRMOVQ, IRMMOVQ, IMRMOVQ,
 * -       IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ };
 * +       IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ, IIADDQ };
 * 
 * # Does fetched instruction require a regid byte?
 * bool need_regids =
 *     icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, 
 *          IIRMOVQ, IRMMOVQ, IMRMOVQ, IIADDQ };
 * 
 * # Does fetched instruction require a constant word?
 * bool need_valC =
 * -    icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL };
 * +    icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL, IIADDQ };
 * 
 * ################ Decode Stage    ###################################
 * 
 * @@ -129,7 +129,7 @@
 * 
 * ## What register should be used as the B source?
 * word srcB = [
 * -    icode in { IOPQ, IRMMOVQ, IMRMOVQ } : rB;
 * +    icode in { IOPQ, IRMMOVQ, IMRMOVQ, IIADDQ } : rB;
 *     icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
 *     1 : RNONE;  # Don't need register
 * ];
 * @@ -137,7 +137,7 @@
 * ## What register should be used as the E destination?
 * word dstE = [
 *     icode in { IRRMOVQ } && Cnd : rB;
 * -    icode in { IIRMOVQ, IOPQ} : rB;
 * +    icode in { IIRMOVQ, IOPQ, IIADDQ } : rB;
 *     icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
 *     1 : RNONE;  # Don't write any register
 * ];
 * @@ -153,7 +153,7 @@
 * ## Select input A to ALU
 * word aluA = [
 *     icode in { IRRMOVQ, IOPQ } : valA;
 * -    icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ } : valC;
 * +    icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IIADDQ } : valC;
 *     icode in { ICALL, IPUSHQ } : -8;
 *     icode in { IRET, IPOPQ } : 8;
 *     # Other instructions don't need ALU
 * ];
 * @@ -162,7 +162,7 @@
 * ## Select input B to ALU
 * word aluB = [
 *     icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL,
 *           IPUSHQ, IRET, IPOPQ, IIADDQ } : valB;
 *     icode in { IRRMOVQ, IIRMOVQ } : 0;
 *     # Other instructions don't need ALU
 * ];
 * @@ -174,7 +174,7 @@
 * ];
 * 
 * ## Should the condition codes be updated?
 * -bool set_cc = icode in { IOPQ };
 * +bool set_cc = icode in { IOPQ, IIADDQ };
 * 
 * ################ Memory Stage    ###################################
 * 
 */