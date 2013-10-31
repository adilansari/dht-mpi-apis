
#ifndef CMPI_TYPES_H
#define CMPI_TYPES_H

#define CMPI_KEY_LENGTH 64
#define CMPI_VALUE_LENGTH (64*1024)

/**
   CMPI method return error codes.
*/
typedef enum
{
  /**
     Successful return code.
  */
  CMPI_SUCCESS = 0,

  /**
     Unknown error.
  */
  CMPI_ERROR_UNKNOWN = 1,

  /**
     Record does not exist.
  */
  CMPI_DOESNT_EXIST = 2,

  /**
     Query target is in a fault state.
  */
  CMPI_ERROR_FAULT = 3,

  /**
     Could not find port to attach.
  */
  CMPI_ERROR_SERVICENAME = 4,

  /**
     Insufficient neighbors or poorly connected network.
  */
  CMPI_ERROR_NEIGHBORS = 5
} CMPI_RETURN;

#endif
