{
  'targets': [
    {
      'target_name': 'QQWry',
      'sources': [ 'QQWry.cc', 'IPInfo.cpp' ],
      'include_dirs': ['deps'],
      'msvs_settings': {
        'VCLinkerTool': {
          'IgnoreDefaultLibraryNames': [
            'libcmt.lib',
          ],
          'AdditionalDependencies' : ['../deps/libiconvStatic.lib']
        },
      }
    }
  ]
}