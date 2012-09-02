#include "error.h"

const ERROR_CODE ERR_NONE = 0;

/* Generic errors */
const ERROR_CODE ERR_MEMFAIL = 1;;

/* Configuration errors */
#define ERR_CFG 0x000100
const ERROR_CODE ERR_CFG_NOARGS      = ERR_CFG + 1;
const ERROR_CODE ERR_CFG_HELPREQUEST = ERR_CFG + 2;

/* URL errors */
#define ERR_URL 0x000200
const ERROR_CODE ERR_URL_CANNOTPARSE = ERR_URL + 1;
const ERROR_CODE ERR_URL_NOTENOUGHDATA = ERR_URL + 2;
