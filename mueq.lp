min: a1 + a2;
C1: X11 =1;
C2: X21 =1;
C3: X32 =1;
C4: X43 =1;
C5: X54 =1;
C6: X61  + X62 =1;
C7: X72  + X73 =1;
C8: X81  + X82  + X83 =1;
C9: X92  + X93  + X94 =1;
C10: X102  + X103 =1;
C11: X112  + X113  + X114 =1;
C12: X125 =1;
 
C13:  X11 + X21 + X61 + X81 -a1<=0;
C14:  X32 + X62 + X72 + X82 -a1<=0;
C15:  X73 + X83 -a1<=0;
C16:  X92 + X102 + X112 -a2<=0;
C17:  X43 + X93 + X103 + X113 -a2<=0;
C18:  X54 + X94 + X114 -a2<=0;

int a1, a2, X61, X62, X72, X73, X81, X82, X83, X92, X93, X94, X102, X103, X112, X113, X114;