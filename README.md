# A tiny and flexible log library for C/C++. Written in C.

### Platform tested: linux, win32, wince, mac

## Usage:
####0.  `#include "ezlog.h"` then Initialize ezlog. e.g.

    ezlog_init_default();

This will display the message to console, and use the format "YY%-%MM%-%DD% %hh%:%mm%:%ss% {tid:%tid% pid:%pid}[%file%] %func% @%line%: %msg".

If you don't want to log to console, you can call `ezlog_set_default_appender(0)` after `ezlog_init_default()`, or just don't call `ezlog_init_default()` at all.

If you need to log to a file

    ezlog_registerAppender(file_appender("ezlog.txt", Append));

If you want to use other target, you can define your own appender.


    typede (*appender)(consttruct {
        void (*handle)(const char* msg, void* opaque);
        void (*close)(void* opaque);
        void *opaque;
    } appender_t;

e.g.

appender_t.handle is

    void widget_appender_handle(const char* msg, void *opaque) {
        ((Widget*)opaque)->showText(msg);
    }

appender_t.opaque is your widget instance

and for android

    void android_appender_handle(const char* msg, void *) {
        __android_log_print(ANDROID_LOG_DEBUG, "%s", msg); //TODO: logtag
    }


####1. Setup log message format (aka layout).
ezlog supports global layout and appender specified layout.  
If you use ezlog_init_default() before and wanna keep the defaul  
format, skip this step. Log message will use the last format you defined.  

**Key words**: YY, MM, DD, hh, mm, ss, ms, tid, pid, file, func, line, msg.  
It's easy to understand what they mean, so I will not explain these. A key word must between two "%",  
except the first and the last. If string between "%" is not a key word, ezlog will print it with out any change.  
If use key word func, the complete function name including return type, class, parameters will be supported  
if compiled with gcc.  

set the global layout:  

    ezlog_set_global_layout("MM%-%DD% %hh%:%mm%:%ss% [tid:%tid%]-[%file%] %func% @%line%: %msg");

or the deprecated one  

    ezlog_init_layout("MM%-%DD% %hh%:%mm%:%ss% [tid:%tid%]-[%file%] %func% @%line%: %msg");


If you use `ezlog_init_default()`, skip this step.

Set an appender's layout:

    void ezlog_set_appender_with_layout(appender handle, const char* format);

This function will add the appender if not exists.


####2.  Start your logging. You can add additional messages. It supports printf like format. If nothing  
else you want to put, just keep the parameter empty.

    ezlog_debug("Hello, cruel world!");  
    ezlog_warn("Damn! %s", __DATE__);  
    ezlog_debug();                            //Only display the formated keywords message you defined  


####3. Clean up. If you use the gcc/clang toolchain (VC not supported yet), it will be called after main() auto.

    ezlog_fini();


## Qt Wrapper

Qt's logging functions such as qDebug(), qWarning() are designed customizable with your own message handler. This wrapper use ezlog as the handler.  
It's very easy to use. just `#include "qtezlog.h"`, and call

    ezlog::QtEZLog::install();

Then qDebug(), qWarning() will be formated with ezlog's default layout.  
TO restore the Qt's default, call

    ezlog::QtEZLog::uninstall();

To use a new layout, call

    ezlog::QtEZLog::setLayout(const QString& fmt);


>> wbsecg1@gmail.com  
>> 20120224  
>> Last updated: 20121202  
