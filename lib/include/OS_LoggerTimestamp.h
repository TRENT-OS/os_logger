/* Copyright (C) 2020, HENSOLDT Cyber GmbH */

/**
 * @file
 * @brief Timestamp interface and implementation.
 */
#pragma once

/**
 * @defgroup    timestamp Timestamp interface and implementation
 *
 * @brief       Helper object to create or convert a timestamp.
 * @details     This object contain to create or convert a timestamp.
 *              \n
 *              It can be transformed a timestamp to human readable format and vice verse.
 *              Further it can be create a timestamp from the macro "__DATE__" and "__TIME".
 *              \n
 *              This class has been created for a time server. So the time server hosts the timestamp object as singleton.
 *
 * @ingroup     seos_logger_server, seos_logger_client
*/
#include <stdint.h>
#include <stdbool.h>


/**
 * @details Timestamp_t defines the class datatype for timestamp.
 *
 * @ingroup timestamp
*/
typedef struct Timestamp_t Timestamp_t;
/**
 * @details Time_t defines the class datatype for human readable format.
 *
 * @ingroup timestamp
*/
typedef struct Time_t Time_t;


/**
 * @details Timestamp_dtorT defines the interface for function pointer to
 *          destructor.
 *
 * @ingroup timestamp
*/
typedef void
(*Timestamp_dtorT)(void);


/**
 * @details Timestamp_create_timestampT defines the interface for functions
 *          pointer to create a timestamp.
 *
 * @param   date:   string from the macro "__DATE"
 * @param   time:   string from the macro "__TIME"
 *
 * @return  an status code
 *
 * @ingroup timestamp
*/
typedef bool
(*Timestamp_create_timestampT)(const char* date, const char* time);


/**
 * @details Timestamp_get_timeT defines the interface for functions pointer to
 *          convert a timestamp in human readable format.
 *
 * @param   t_stamp:    pointer to timestamp
 * @param   hours:      offset in hours for time shift
 * @param   tm:         destination struct to save the converted time
 *
 * @return  an status code
 *
 * @ingroup timestamp
*/
typedef bool
(*Timestamp_get_timeT)(Timestamp_t* t_stamp, uint8_t hours, Time_t* tm);


/**
 * @details Timestamp_get_timestampT defines the interface for functions pointer
 *          to convert from human readable format into timestamp.
 *
 * @param   tm:         source time
 * @param   t_stamp:    destination struct to save the timestamp
 *
 * @return  an status code
 *
 * @ingroup timestamp
*/
typedef bool
(*Timestamp_get_timestampT)(Time_t* tm, Timestamp_t* t_stamp);


/**
 * @details Time_t contain the parameters for human readable format.
 *
 * @ingroup timestamp
*/
struct Time_t
{
    uint8_t  sec;   /**< second */
    uint8_t  min;   /**< minute */
    uint8_t  hour;  /**< hour */
    uint8_t  day;   /**< day in month */
    uint8_t  month; /**< month */
    uint16_t year;  /**< year */
};


/**
 * @details Timestamp_Vtable contain the member functions to his class.
 *
 * @ingroup timestamp
*/
typedef struct
{
    Timestamp_dtorT             dtor;
    Timestamp_create_timestampT create_timestamp;
    Timestamp_get_timeT         get_time;
    Timestamp_get_timestampT    get_timestamp;
} Timestamp_Vtable;


/**
 * @details Timestamp_t contain the vtable to his class.
 *
 * @ingroup timestamp
*/
struct Timestamp_t
{
    uint64_t                timestamp; /**< timestamp */
    const Timestamp_Vtable* vtable;    /**< vtable */
};


/**
 * @details %get_instance_Timestamp is a getter to instantiate this object.
 *
 *          This is an singleton implementation. It will be internally create a
 *          timestamp object.
 *
 *          If the object has been successfully created, the pointer to this
 *          object will returned, and NULL pointer in the other case.
 *
 * @return  pointer to Timestamp_t object
 *
 * @retval  not NULL, if all allright
 *              NULL, if an error has been occurred
 *
 * @ingroup timestamp
*/
Timestamp_t*
get_instance_Timestamp(void);


/**
 * @details %Timestamp_dtor is the destructor.
 *
 * @ingroup timestamp
*/
void
Timestamp_dtor(void);


/**
 * @details %Timestamp_create_timestamp create a timestamp from the given
 *          strings "__DATE__" and "__TIME__".
 *
 * @param   date:   string from the macro "__DATE"
 * @param   time:   string from the macro "__TIME"
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if an error has been occurred
 *
 * @ingroup timestamp
*/
bool
Timestamp_create_timestamp(const char* date, const char* time);


/**
 * @details %Timestamp_get_time convert a timestamp in human readable format.
 *
 * @param   t_stamp:    pointer to timestamp
 * @param   hours:      offset in hours for time shift
 * @param   tm:         destination struct to save the converted time
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if an error has been occurred
 *
 * @ingroup timestamp
*/
bool
Timestamp_get_time(Timestamp_t* t_stamp, uint8_t hours, Time_t* tm);


/**
 * @details %Timestamp_get_timestamp convert from human readable format into
 *          timestamp.
 *
 * @param   tm:         source time
 * @param   t_stamp:    destination struct to save the timestamp
 *
 * @return  an status code
 *
 * @retval  true,  if all allright
 *          false, if an error has been occurred
 *
 * @ingroup timestamp
*/
bool
Timestamp_get_timestamp(Time_t* tm, Timestamp_t* t_stamp);
