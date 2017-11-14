//
//  Trace.h
//  MappingSdk
//
//  Created by Marcin Fraczak on 4/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

// Borrowed from: http://stackoverflow.com/questions/1667994/best-practices-for-error-logging-and-or-reporting-for-iphone

#ifndef MappingSdk_Trace_h
#define MappingSdk_Trace_h

#ifndef NSERROR_WITH_MSG
#define NSERROR_WITH_MSG( msg ) \
[NSError errorWithDomain:@"com.maptext.MappingSdk" code:-1 userInfo:@{NSLocalizedDescriptionKey:msg}]
#endif

/**
 * Use this class to configure the level of trace left by the SDK.
 */
class TraceConfig
{
public:

    /**
     * Different levels of logging. They are inclusive in ascending order, i.e.
     * TraceLevel_Info also enables all traces left on TraceLevel_Debug level etc.
     */
    enum TraceLevel
    {
        /**
         * Max level of tracing, including purely informational information.
         */
        TraceLevel_Info,
        /**
         * Debug level, more detailed messages helpful in debugging complex 
         * fragments of SDK.
         */
        TraceLevel_Debug,
        /**
         * Normal level, more important debug messages.
         */         
        TraceLevel_Normal,
        /**
         * Log non-critical warnings.
         */
        TraceLevel_Warning,
        /**
         * Log only critical errors.
         */
        TraceLevel_Error,
        /**
         * Leave no trace.
         */
        TraceLecel_None
    };
    
    /**
     * Retrieve the current trace level.
     * @return - The trace level value.
     */
    static TraceLevel GetTraceLevel()
    {
        return m_traceLevel;
    }
    
    /**
     * Set a new trace level.
     * @param level - New level of tracing.
     */
    static void SetTraceLevel( TraceLevel level )
    {
        m_traceLevel = level;
    }

private:
    static TraceLevel m_traceLevel;
};


/*****************************************************************************/
/* Entry/exit trace macros                                                   */
/*****************************************************************************/
#define TRC_ENTRY   if ( TraceConfig::GetTraceLevel() <= TraceConfig::TraceLevel_Info ) \
    NSLog(@"ENTRY: %s:%d:", __FILE__,__LINE__);
#define TRC_EXIT     if ( TraceConfig::GetTraceLevel() <= TraceConfig::TraceLevel_Info ) \
    NSLog(@"EXIT:  %s:%d:", __FILE__,__LINE__);

/*****************************************************************************/
/* Debug trace macros                                                        */
/*****************************************************************************/
#define TRC_DBG(A, ...)  if ( TraceConfig::GetTraceLevel() <= TraceConfig::TraceLevel_Debug ) \
    NSLog(@"DEBUG: %s:%d:%@", __FILE__,__LINE__,[NSString stringWithFormat:A, ## __VA_ARGS__]);

#define TRC_NRM(A, ...)  if ( TraceConfig::GetTraceLevel() <= TraceConfig::TraceLevel_Normal ) \
    NSLog(@"NORMAL:%s:%d:%@", __FILE__,__LINE__,[NSString stringWithFormat:A, ## __VA_ARGS__]);

#define TRC_ALT(A, ...)  if ( TraceConfig::GetTraceLevel() <= TraceConfig::TraceLevel_Warning ) \
    NSLog(@"ALERT: %s:%d:%@", __FILE__,__LINE__,[NSString stringWithFormat:A, ## __VA_ARGS__]);

#define TRC_ERR(A, ...)  if ( TraceConfig::GetTraceLevel() <= TraceConfig::TraceLevel_Error ) \
    NSLog(@"ERROR: %s:%d:%@", __FILE__,__LINE__,[NSString stringWithFormat:A, ## __VA_ARGS__]);

#define TRC_DTP_ERR(dtpError) if ( TraceConfig::GetTraceLevel() <= TraceConfig::TraceLevel_Error ) \
    NSLog(@"ERROR: %s:%d: Caught DTP error originating from %s:%d: %s", __FILE__,__LINE__, dtpError.GetFile(), dtpError.GetLine(), dtpError.GetErrorMessage() );

#endif
