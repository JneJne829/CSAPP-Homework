/*
 * A.
 * 
 *  104 +----------------+
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *   64 +----------------+ <=== %rdi
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *   32 +----------------+
 *      |       z        |
 *   24 +----------------+
 *      |    rsp + 24    |
 *   16 +----------------+
 *      |       y        |
 *    8 +----------------+
 *      |       x        |
 *    0 +----------------+ <=== %rsp
 * 
 * B.
 * 
 * %rsp + 64 in %rdi
 * 
 * C.
 * 
 * %rsp + offset
 * 
 * D.
 * 
 * Pass adress %rsp + 64 to process,
 * store data from here,
 * finlly return this adress.
 * 
 * E.
 * 
 *  104 +----------------+
 *      |                |
 *      |                |
 *   88 +----------------+
 *      |       z        |
 *   80 +----------------+
 *      |       x        |
 *   72 +----------------+
 *      |       y        |
 *   64 +----------------+ <=== %rdi , %rax
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *      |                |
 *   32 +----------------+
 *      |       z        |
 *   24 +----------------+
 *      |    rsp + 24    |
 *   16 +----------------+
 *      |       y        |
 *    8 +----------------+
 *      |       x        |
 *    0 +----------------+ <=== %rsp
 * 
 * F.
 * 
 * caller find space and pass space address to callee,
 * callee store data on this space area and
 * return this address.
 * 
 */