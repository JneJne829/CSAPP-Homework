/* 
 * 
 * --- origin-pipe-1w.hcl    2021-02-25 07:26:33.309259378 +0000
 * +++ pipe-1w.hcl    2021-02-25 07:26:33.309259378 +0000
 * @@ -157,6 +157,7 @@
 * ## so that it will be IPOP2 when fetched for the second time.
 *  word f_icode = [
 *         imem_error : INOP;
 * +       D_icode == IPOPQ : IPOP2;
 *         1: imem_icode;
 *  ];
 * 
 * @@ -169,7 +170,7 @@
 * # Is the instruction valid?
 *  bool instr_valid = f_icode in 
 *         { INOP, IHALT, IRRMOVQ, IIRMOVQ, IRMMOVQ, IMRMOVQ,
 * -       IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ };
 * +       IOPQ, IJXX, ICALL, IRET, IPUSHQ, IPOPQ, IPOP2 };
 * 
 * # Determine the status code for the fetched instruction
 *  word f_stat = [
 * @@ -182,7 +183,7 @@
 * # Does the fetched instruction require a regid byte?
 *  bool need_regids =
 *         f_icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, 
 * -                     IIRMOVQ, IRMMOVQ, IMRMOVQ };
 * +                     IIRMOVQ, IRMMOVQ, IMRMOVQ, IPOP2 };
 * 
 * # Does the fetched instruction require a constant word?
 *  bool need_valC =
 * @@ -192,6 +193,7 @@
 *  word f_predPC = [
 *         f_icode in { IJXX, ICALL }: f_valC;
 *         ## 1W: Want to refetch popq one time
 * +       f_icode == IPOPQ : f_pc;
 *         1: f_valP;
 *  ];
 * 
 * @@ -204,14 +206,14 @@
 * ## What register should be used as the A source?
 *  word d_srcA = [
 *         D_icode in { IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ }: D_rA;
 * -       D_icode in { IPOPQ, IRET }: RRSP;
 * +       D_icode in { IRET }: RRSP;
 *         1: RNONE; # Don't need a register
 *  ];
 * 
 * ## What register should be used as the B source?
 *  word d_srcB = [
 *         D_icode in { IOPQ, IRMMOVQ, IMRMOVQ }: D_rB;
 * -       D_icode in { IPUSHQ, IPOPQ, ICALL, IRET }: RRSP;
 * +       D_icode in { IPUSHQ, IPOPQ, IPOP2, ICALL, IRET }: RRSP;
 *         1: RNONE;  # Don't need a register
 *  ];
 * 
 * @@ -224,7 +226,7 @@
 * 
 * ## What register should be used as the M destination?
 *  word d_dstM = [
 * -       D_icode in { IMRMOVQ, IPOPQ }: D_rA;
 * +       D_icode in { IMRMOVQ, IPOP2 }: D_rA;
 *         1: RNONE;  # Don't write any register
 *  ];
 * 
 * @@ -255,7 +257,7 @@
 *  word aluA = [
 *         E_icode in { IRRMOVQ, IOPQ }: E_valA;
 *         E_icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ }: E_valC;
 * -       E_icode in { ICALL, IPUSHQ }: -8;
 * +       E_icode in { ICALL, IPUSHQ, IPOP2 }: -8;
 *         E_icode in { IRET, IPOPQ }: 8;
 *         # Other instructions don't need ALU
 *  ];
 * @@ -263,7 +265,7 @@
 * ## Select input B to ALU
 *  word aluB = [
 *         E_icode in { IRMMOVQ, IMRMOVQ, IOPQ, ICALL, 
 * -                     IPUSHQ, IRET, IPOPQ }: E_valB;
 * +                     IPUSHQ, IRET, IPOPQ, IPOP2 }: E_valB;
 *         E_icode in { IRRMOVQ, IIRMOVQ }: 0;
 *         # Other instructions don't need ALU
 *  ];
 * @@ -292,13 +294,13 @@
 * 
 * ## Select memory address
 *  word mem_addr = [
 * -       M_icode in { IRMMOVQ, IPUSHQ, ICALL, IMRMOVQ }: M_valE;
 * -       M_icode in { IPOPQ, IRET }: M_valA;
 * +       M_icode in { IRMMOVQ, IPUSHQ, ICALL, IMRMOVQ, IPOP2 }: M_valE;
 * +       M_icode in { IRET }: M_valA;
 *         # Other instructions don't need an address
 *  ];
 * 
 * ## Set read control signal
 *  bool mem_read = M_icode in { IMRMOVQ, IRET, IPOP2 };
 * 
 * ## Set write control signal
 *  bool mem_write = M_icode in { IRMMOVQ, IPUSHQ, ICALL };
 * @@ -350,7 +352,7 @@
 *  bool F_bubble = 0;
 *  bool F_stall =
 *         # Conditions for a load/use hazard
 * -       E_icode in { IMRMOVQ, IPOPQ } &&
 * +       E_icode in { IMRMOVQ, IPOP2 } &&
 *          E_dstM in { d_srcA, d_srcB } ||
 *         # Stalling at fetch while ret passes through the pipeline
 *         IRET in { D_icode, E_icode, M_icode };
 * @@ -359,7 +361,7 @@
 *  # At most one of these can be true.
 *  bool D_stall = 
 *         # Conditions for a load/use hazard
 * -       E_icode in { IMRMOVQ, IPOPQ } &&
 * +       E_icode in { IMRMOVQ, IPOP2 } &&
 *          E_dstM in { d_srcA, d_srcB };
 * 
 *  bool D_bubble =
 * @@ -367,7 +369,7 @@
 *         (E_icode == IJXX && !e_Cnd) ||
 *         # Stalling at fetch while ret passes through the pipeline
 *         # but not the condition for a load/use hazard
 * -       !(E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB }) &&
 * +       !(E_icode in { IMRMOVQ, IPOP2 } && E_dstM in { d_srcA, d_srcB }) &&
 *         # 1W: This condition will change
 *         IRET in { D_icode, E_icode, M_icode };
 * 
 * @@ -378,7 +380,7 @@
 *         # Mispredicted branch
 *         (E_icode == IJXX && !e_Cnd) ||
 *         # Conditions for a load/use hazard
 * -       E_icode in { IMRMOVQ, IPOPQ } &&
 * +       E_icode in { IMRMOVQ, IPOP2 } &&
 *          E_dstM in { d_srcA, d_srcB};
 * 
 *  # Should I stall or inject a bubble into Pipeline Register M?
 * 
 */