# Local variables:
# mode: python
# End:
{
    'targets': [
      {
            'target_name': 'test',
            'type': 'executable',
            'sources': [
                'test.cc',
                'src/world/world.cc',
                'src/assets/assets.cc',
                ],
            'cflags' : ["-g"],
            'include_dirs': [
                '.',
                'src',
                ],
            'conditions' : [
                 ['OS=="linux"',
                  {
                      'sources' : [
                          'src/rpi/terminal.cc',
                          'src/tests/rpi/test-terminal.cc',
                          ],
                      'include_dirs' : [
                          '/opt/vc/include',
                          '/opt/vc/include/interface/vcos/pthreads',
                          '/usr/include/eigen3',
                          'rpi-gmock-1.6.0/include',
                          'rpi-gmock-1.6.0/gtest/include',
                          ],
                      'libraries' : [
                          '-lgmock',
                          '-lgtest',
                          '-lpthread',
                          '-lboost_filesystem',
                          '-lboost_system',
                          '-lpng12',
                          ],
                      'ldflags': [
                          '-L/home/deets/projects/raspberry-racer/cpp/rpi-gmock-1.6.0/gtest',
                          '-L/home/deets/projects/raspberry-racer/cpp/rpi-gmock-1.6.0',
                          ],
                      },
                  ],
                 ['OS=="mac"',
                  {
                      'xcode_settings': {
                          'ARCHS': [ 'x86_64' ],
                          },
                      'include_dirs' : [
                          '/usr/local/include',
                          # this required a link in MonkVG -> VG
                          "/Users/deets/software/vc/MonkVG/include",
                          'osx-gmock-1.6.0/include',
                          'osx-gmock-1.6.0/gtest/include',
                          ],
                      'libraries' : [
                          '-L/usr/local/lib',
                          '-L/Users/deets/projects/private/raspberry-racer/cpp/osx-gmock-1.6.0',
                          '-L/Users/deets/projects/private/raspberry-racer/cpp/osx-gmock-1.6.0/gtest',

                          '-lgmock',
                          '-lgtest',
                          '-lpthread',
                          '-lboost_filesystem-mt',
                          '-lboost_system-mt',
                          '-lpng15',
                          ],
                      },
                  ],

                ],
            },
        {
            'target_name': 'rracer',
            'type': 'executable',
            'cflags' : ["-g"],
            'sources': [
                'main.cc',
                'src/assets/assets.cc',
                'openvg-adapter.cc',
                ],
            'include_dirs': [
                '.',
                'src',
                ],
            'conditions' : [
                 ['OS=="linux"',
                  {
                      'sources' : [
                          'src/rpi/linux-event-pump.cc',
                          'src/rpi/posix-adapter-impl.cc',
                          'src/rpi/rpi-window-adapter.cc',
                          'src/rpi/oglinit.cc',
                          'src/rpi/terminal.cc',
                          ],
                      'include_dirs' : [
                          '/opt/vc/include',
                          '/opt/vc/include/interface/vcos/pthreads',
                          '/usr/include/eigen3',
                          ],
                      'libraries' : [
                          '-lbcm_host',
                          '-lGLESv2',
                          '-lboost_filesystem',
                          '-lboost_system',
                          '-lpng12',
                          ],
                      'ldflags': [
                          '-L/opt/vc/lib',
                          ],
                      },
                  ],
                 ['OS=="mac"',
                  {
                      'xcode_settings': {
                          'ARCHS': [ 'x86_64' ],
                          },
                      'include_dirs' : [
                          '/usr/local/include',
                          # this required a link in MonkVG -> VG
                          "/Users/deets/software/vc/MonkVG/include",
                          ],
                      'libraries' : [
                          '-L/usr/local/lib',
                          '-lboost_filesystem-mt',
                          '-lboost_system-mt',
                          '-lpng15',
                          ],
                      },
                  ],
                ],
            },
        ], # targets
    }
