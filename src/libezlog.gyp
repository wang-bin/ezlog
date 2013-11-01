{
  'targets': [
    {
      'target_name': 'ezlog',
      'type': 'static_library',
      'dependencies': [

      ],
      'defines': [

      ],
      'include_dirs': [
        '.',
      ],
      'sources': [
        'appender.h',
        'ezlog_global.h',
        'ezlog.h',
        'ezmutex.h',
        'eztime.h',
        'layout.h',
        'list.h',
        'prepost.h',
        'private.h',
        'appender.c',
        'ezlog.c',
        'ezmutex.c',
        'eztime.c',
        'layout.c',
        'private.c',
      ],
      'conditions': [
        ['OS=="win"', {

        }, { # OS != "win",
          'cflags': [
            '-fPIC',
          ],
          'ldflags': [
            '-lpthread',
          ],
        }], # OS=="win"
        ['OS=="android"', {
          'ldflags!': [
            '-lpthread',
          ],
        }],
      ],
    },
  ],
}