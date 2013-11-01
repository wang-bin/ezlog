{
  'targets': [
    {
      'target_name': 'tst_ezlog',
      'type': 'executable',
      'dependencies': [
        '../../src/libezlog.gyp:ezlog'
      ],
      'include_dirs': [
        '../../src',
      ],
      'sources': [
        'main.c',
      ],
    },
  ],
}