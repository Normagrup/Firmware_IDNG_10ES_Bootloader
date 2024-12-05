// DISTRIBUCIÓN DE BANCOS DE MEMORIA

// BANCO 0 CONFIGURACION DE LA PASARELA -> config.c
// BANCO 1 CONFIGURACION DE LAZO -> daliLoop.c
// BANCO 2 HISTORICO
// BANCO 3 HISTORICO
// BANCO 4 STORAGE 0004 0000h 0004 FFFFh















void m25pWriteEnable (void);
void m25pWriteDisable (void);
void m25pWriteBytes (unsigned char startAddrH, unsigned char startAddrM, unsigned char startAddrL, unsigned char *buffer, unsigned int size);
void m25pReadBytes (unsigned char startAddrH, unsigned char startAddrM, unsigned char startAddrL, unsigned char *buffer, unsigned int size);
unsigned char m25pStatusRead (void);
void m25pSectorErase (unsigned char startAddrH, unsigned char startAddrM, unsigned char startAddrL);
