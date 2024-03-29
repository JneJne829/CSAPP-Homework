/* 
 * 
 *  
 * 
 * 1.
 * 
 * E_icode in { IMRMOVQ, IPOPQ } &&
 * (
 *   E_dstM == d_srcB ||
 *   (
 *     E_dstM == d_srcA && !(D_icode in { IRMMOVQ, IPUSHQ })
 *   )
 * );
 * 
 * 2.
 * 
 * 
--- origin-pipe-lf.hcl	2021-02-25 07:26:33.309259378 +0000
+++ pipe-lf.hcl	2021-02-25 07:26:33.309259378 +0000
@@ -271,6 +271,7 @@
 ##   from memory stage when appropriate
 ## Here it is set to the default used in the normal pipeline
 word e_valA = [
+	E_icode in { IRMMOVQ, IPUSHQ } && E_srcA == M_dstM : m_valM;
 	1 : E_valA;  # Use valA from stage pipe register
 ];
 
@@ -329,7 +330,13 @@
 bool F_stall =
 	# Conditions for a load/use hazard
 	## Set this to the new load/use condition
-	0 ||
+	E_icode in { IMRMOVQ, IPOPQ } &&
+  (
+    E_dstM == d_srcB ||
+    (
+      E_dstM == d_srcA && !(D_icode in { IRMMOVQ, IPUSHQ })
+    )
+  ) ||
 	# Stalling at fetch while ret passes through pipeline
 	IRET in { D_icode, E_icode, M_icode };
 
@@ -338,15 +345,29 @@
 bool D_stall = 
 	# Conditions for a load/use hazard
 	## Set this to the new load/use condition
-	0; 
+	E_icode in { IMRMOVQ, IPOPQ } &&
+  (
+    E_dstM == d_srcB ||
+    (
+      E_dstM == d_srcA && !(D_icode in { IRMMOVQ, IPUSHQ })
+    )
+  );
 
 bool D_bubble =
 	# Mispredicted branch
 	(E_icode == IJXX && !e_Cnd) ||
 	# Stalling at fetch while ret passes through pipeline
 	# but not condition for a load/use hazard
-	!(E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB }) &&
-	  IRET in { D_icode, E_icode, M_icode };
+	!(
+		E_icode in { IMRMOVQ, IPOPQ } &&
+		(
+		E_dstM == d_srcB ||
+		(
+			E_dstM == d_srcA && !(D_icode in { IRMMOVQ, IPUSHQ })
+			)
+		)
+	) &&
+	IRET in { D_icode, E_icode, M_icode };
 
 # Should I stall or inject a bubble into Pipeline Register E?
 # At most one of these can be true.
@@ -356,7 +377,13 @@
 	(E_icode == IJXX && !e_Cnd) ||
 	# Conditions for a load/use hazard
 	## Set this to the new load/use condition
-	0;
+	E_icode in { IMRMOVQ, IPOPQ } &&
+	(
+		E_dstM == d_srcB ||
+		(
+			E_dstM == d_srcA && !(D_icode in { IRMMOVQ, IPUSHQ })
+		)
+	);
 
 # Should I stall or inject a bubble into Pipeline Register M?
 # At most one of these can be true.

 * 
 * 
 */