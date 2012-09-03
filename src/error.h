#ifndef CLIHT_ERROR_H_
#define CLIHT_ERROR_H_

typedef unsigned int ERROR_CODE;

#define ERR_NONE 0

/* Generic errors */
#define ERR_MEMFAIL           1

/* Configuration errors */
#define ERR_CFG               0x000100
#define ERR_CFG_NOARGS        ERR_CFG + 1
#define ERR_CFG_HELPREQUEST   ERR_CFG + 2

/* URL errors */
#define ERR_URL               0x000200
#define ERR_URL_CANNOTPARSE   ERR_URL + 1
#define ERR_URL_NOTENOUGHDATA ERR_URL + 2

/* HTTP errors */
#define ERR_HTTP              0x000300
#define ERR_HTTP_BLDFAIL_HDR  ERR_HTTP + 1
#define ERR_HTTP_BLDFAIL_LINE ERR_HTTP + 2
#define ERR_HTTP_BLDFAIL_RQST ERR_HTTP + 3

/* Net errors */
#define ERR_NET               0x000400
#define ERR_NET_ADDRFAIL      ERR_NET + 1
#define ERR_NET_CONNFAIL      ERR_NET + 2
#define ERR_NET_SOCKNOTVALID  ERR_NET + 3
#define ERR_NET_SENDFAIL      ERR_NET + 4
#define ERR_NET_RECVFAIL      ERR_NET + 5
#define ERR_NET_CONNCLOSED    ERR_NET + 6

/* File I/O errors */
#define ERR_FILE              0x000500
#define ERR_FILE_OPENFAIL     ERR_FILE + 1
#define ERR_FILE_WRITEFAIL    ERR_FILE + 2

void err_print(ERROR_CODE err);

#endif // CLIHT_ERROR_H_
