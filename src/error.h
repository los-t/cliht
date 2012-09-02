#ifndef CLIHT_ERROR_H_
#define CLIHT_ERROR_H_

typedef unsigned int ERROR_CODE;

extern const ERROR_CODE ERR_NONE;

extern const ERROR_CODE ERR_MEMFAIL;

extern const ERROR_CODE ERR_CFG;
extern const ERROR_CODE ERR_CFG_NOARGS;
extern const ERROR_CODE ERR_CFG_HELPREQUEST;

extern const ERROR_CODE ERR_URL;
extern const ERROR_CODE ERR_URL_CANNOTPARSE;
extern const ERROR_CODE ERR_URL_NOTENOUGHDATA;

#endif // CLIHT_ERROR_H_
