# Local variables:
# mode: python
# End:
{
    'targets': [
      {
            'target_name': 'test',
            'type': 'executable',
            'sources': [
                'terminal.cc',
                'test.cc',
                'src/world/world.cc',
                'src/assets/assets.cc',
                ],
            'cflags' : ["-g"],
            'include_dirs': [
                '.',
                'src',
                'gmock-1.6.0/include',
                'gmock-1.6.0/gtest/include',
                '/opt/vc/include',
                '/opt/vc/include/interface/vcos/pthreads',
                '/usr/include/eigen3',
                ],
            'libraries' : [
                '-lgmock',
                '-lgtest',
                '-lpthread',
                '-lboost_filesystem',
                '-lboost_system',
                '-lpng12',
                ],
            'conditions' : [
                 ['OS=="linux"',
                  {
                      'ldflags': [
                          '-L/home/deets/projects/raspberry-racer/cpp/gmock-1.6.0/gtest',
                          '-L/home/deets/projects/raspberry-racer/cpp/gmock-1.6.0',
                          ],
                      }
                  ],
                ],
            },
        {
            'target_name': 'rracer',
            'type': 'executable',
            'cflags' : ["-g"],
            'sources': [
                'linux-event-pump.cc',
                'main.cc',
                'posix-adapter-impl.cc',
                'terminal.cc',
                'rpi-window-adapter.cc',
                'oglinit.cc',
                'openvg-adapter.cc',
                'src/assets/assets.cc',
                ],
            'include_dirs': [
                '.',
                'src',
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
            'conditions' : [
                 ['OS=="linux"',
                  {
                      'ldflags': [
                          '-L/opt/vc/lib',
                          ],
                      }
                  ],
                ],
            }
        ],
    }
