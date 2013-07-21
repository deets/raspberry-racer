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
            'sources': [
                'terminal.cc',
                'main.cc',
                'posix-adapter-impl.cc',
                ],
            }
        ],
    }
