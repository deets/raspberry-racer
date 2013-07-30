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
                'posix-adapter.cc',
                ],
            'cflags' : ["-g", "-H"],
            'include_dirs': [
                'gmock-1.6.0/include',
                'gmock-1.6.0/gtest/include',
                ],
            'libraries' : [
                '-lgmock',
                '-lgtest',
                '-lpthread',
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
                'rpi-adapter-impl.cc',
                'posix-adapter.cc',
                'terminal.cc',
                'rpi-window-adapter.cc',
                'oglinit.cc',
                'openvg-adapter.cc',
                ],
            'include_dirs': [
                '/opt/vc/include',
                '/opt/vc/include/interface/vcos/pthreads',
                ],
            'libraries' : [
                '-lbcm_host',
                '-lGLESv2',
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
