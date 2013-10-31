/*
 * (C) 2001 Clemson University and The University of Chicago
 *
 * See COPYING in top-level directory.
 */

/** \defgroup gossip gossip logging interface
 *
 * This is a basic application logging facility.  It uses printf style
 * formatting and provides several mechanisms for output.
 *
 * @{
 */

/** \file
 *
 *  Declarations for the gossip logging interface.
 */

#ifndef __GOSSIP_H
#define __GOSSIP_H

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <sys/time.h>
#ifdef HAVE_EXECINFO_H
#include <execinfo.h>
#endif
#ifdef __GOSSIP_ENABLE_MPI__
#include <mpi.h>
#endif

#ifndef __KERNEL__
#include <stdint.h>
#include <stdarg.h>
#include "syslog.h"
#endif
#include "cmpi-config.h"

// Masks...
#define MASK_ALL     ((uint64_t) -1)
#define MASK_NONE    ((uint64_t) 0)
#define MASK_DEBUG   ((uint64_t) 1 << 1)
#define MASK_MPIRPC  ((uint64_t) 1 << 2)
#define MASK_DHT     ((uint64_t) 1 << 3)
#define MASK_ADTS    ((uint64_t) 1 << 4)
#define MASK_DRIVER  ((uint64_t) 1 << 5)
#define MASK_CMPI    ((uint64_t) 1 << 6)
#define MASK_ARCH    ((uint64_t) 1 << 7)
#define MASK_STORAGE ((uint64_t) 1 << 8)
#define MASK_KDA     ((uint64_t) 1 << 9)
#define MASK_EXAMPLE ((uint64_t) 1 << 10)
#define MASK_CLIENT  ((uint64_t) 1 << 12)

/********************************************************************
 * Visible interface
 */

#define GOSSIP_BUF_SIZE 1024

/* what type of timestamp to place in msgs */
enum gossip_logstamp
{
    GOSSIP_LOGSTAMP_NONE = 0,
    GOSSIP_LOGSTAMP_USEC = 1,
    GOSSIP_LOGSTAMP_DATETIME = 2,
    GOSSIP_LOGSTAMP_THREAD = 3,
    GOSSIP_LOGSTAMP_MPI = 4,
};
#define GOSSIP_LOGSTAMP_DEFAULT GOSSIP_LOGSTAMP_MPI

#define gossip_debug_init() \
do {\
    char* __mask_str; \
    uint64_t __mask; \
    __mask_str = getenv("CMPI_DEBUG_MASK"); \
    __mask = (__mask_str ? gossip_set_debug_mask_human(__mask_str) : 0); \
    gossip_set_debug_mask(1, __mask); \
}while(0)

uint64_t gossip_set_debug_mask_human(
    const char *event_logging);

/* Keep a simplified version for the kmod */
#ifdef __KERNEL__

#ifdef GOSSIP_DISABLE_DEBUG
#define gossip_debug(mask, format, f...) do {} while(0)
#else
extern int gossip_debug_mask;

/* try to avoid function call overhead by checking masks in macro */
#define gossip_debug(mask, format, f...)                  \
do {                                                      \
    if (gossip_debug_mask & mask)                         \
    {                                                     \
        printk(format, ##f);                              \
    }                                                     \
} while(0)
#endif /* GOSSIP_DISABLE_DEBUG */

/* do file and line number printouts w/ the GNU preprocessor */
#define gossip_ldebug(mask, format, f...)                  \
do {                                                       \
    gossip_debug(mask, "%s: " format, __func__ , ##f); \
} while(0)

#define gossip_err printk
#define gossip_lerr(format, f...)                  \
do {                                               \
    gossip_err("%s line %d: " format, __FILE__ , __LINE__ , ##f); \
} while(0)

#else /* __KERNEL__ */

/* stdio is needed by gossip_debug_fp declaration for FILE* */
#include <stdio.h>

int gossip_enable_syslog(
    int priority);
int gossip_enable_stdout(
    void);
int gossip_enable_stderr(
    void);
int gossip_enable_file(
    const char *filename,
    const char *mode);
int gossip_disable(
    void);
int gossip_set_debug_mask(
    int debug_on,
    uint64_t mask);
int gossip_get_debug_mask(
    int *debug_on,
    uint64_t *mask);
int gossip_set_logstamp(
    enum gossip_logstamp ts);

void gossip_backtrace(void);

#ifdef __GNUC__

/* do printf style type checking if built with gcc */
int __gossip_debug(
    uint64_t mask,
    char prefix,
    const char *format,
    ...) __attribute__ ((format(printf, 3, 4)));
int gossip_err(
    const char *format,
    ...) __attribute__ ((format(printf, 1, 2)));
int __gossip_debug_va(
    uint64_t mask,
    char prefix,
    const char *format,
    va_list ap);
int gossip_debug_fp(
    FILE *fp,
    char prefix,
    enum gossip_logstamp ts,
    const char *format,
    ...) __attribute__ ((format(printf, 4, 5)));

#ifdef GOSSIP_DISABLE_DEBUG
#define gossip_debug(mask, format, f...) do {} while(0)
#define gossip_perf_log(format, f...) do {} while(0)
#define gossip_debug_enabled(__m) 0
#else
extern int gossip_debug_on;
extern int gossip_facility;
extern uint64_t gossip_debug_mask;

#define gossip_debug_enabled(__m) \
    (gossip_debug_on && (gossip_debug_mask & __m))

/* try to avoid function call overhead by checking masks in macro */
#define gossip_debug(mask, format, f...)                  \
do {                                                      \
    if ((gossip_debug_on) && (gossip_debug_mask & mask) &&\
        (gossip_facility))                                \
    {                                                     \
        __gossip_debug(mask, '?', format, ##f);           \
    }                                                     \
} while(0)
#define gossip_perf_log(format, f...)                     \
do {                                                      \
    if ((gossip_debug_on) &&                              \
        (gossip_debug_mask & GOSSIP_PERFCOUNTER_DEBUG) && \
        (gossip_facility))                                \
    {                                                     \
        __gossip_debug(GOSSIP_PERFCOUNTER_DEBUG, 'P',     \
            format, ##f);                                 \
    }                                                     \
} while(0)
#define gossip_do(mask, x)                                      \
    if ((gossip_debug_on) && (gossip_debug_mask & mask) &&      \
        (gossip_facility))                                      \
    { x; }

#endif /* GOSSIP_DISABLE_DEBUG */

/* do file and line number printouts w/ the GNU preprocessor */
#define gossip_ldebug(mask, format, f...)                  \
do {                                                       \
    gossip_debug(mask, "%s %s() line %d: " format, __FILE__, __func__ , __LINE__, ##f); \
} while(0)

#define gossip_lerr(format, f...)                  \
do {                                               \
    gossip_err("%s %s() line %d: " format, __FILE__ , __func__, __LINE__ , ##f); \
    gossip_backtrace();                            \
} while(0)
#else /* ! __GNUC__ */

int __gossip_debug(
    uint64_t mask,
    char prefix,
    const char *format,
    ...);
int __gossip_debug_stub(
    uint64_t mask,
    char prefix,
    const char *format,
    ...);
int gossip_err(
    const char *format,
    ...);

#ifdef GOSSIP_DISABLE_DEBUG
#define gossip_debug(__m, __f, f...) __gossip_debug_stub(__m, '?', __f, ##f);
#define gossip_ldebug(__m, __f, f...) __gossip_debug_stub(__m, '?', __f, ##f);
#define gossip_debug_enabled(__m) 0
#else
#define gossip_debug(__m, __f, f...) __gossip_debug(__m, '?', __f, ##f);
#define gossip_ldebug(__m, __f, f...) __gossip_debug(__m, '?', __f, ##f);
#define gossip_debug_enabled(__m) \
            ((gossip_debug_on != 0) && (__m & gossip_debug_mask))

#endif /* GOSSIP_DISABLE_DEBUG */

#define gossip_lerr gossip_err

#endif /* __GNUC__ */

#endif /* __KERNEL__ */

#endif /* __GOSSIP_H */

/* @} */

/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: ts=8 sts=4 sw=4 expandtab
 */
