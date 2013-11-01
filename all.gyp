{
  'targets': [
    {
      'target_name': 'all_targets',
      'type': 'none',
      'dependencies': [
        'src/libezlog.gyp:*',
        'test/tst_ezlog/tst_ezlog.gyp:*',
      ],
    },
  ],
}
