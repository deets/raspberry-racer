# Local variables:
# mode: python
# End:
{
    'target_defaults' : {
        'default_configuration': 'Debug',
        'configurations': {
            'Debug': {
                'xcode_settings': {
                    'OTHER_CFLAGS' : ['-g', '-fno-eliminate-unused-debug-symbols', "-O0"],
#                    'GCC_DEBUGGING_SYMBOLS': 'full',
                    },
                },
            },
        },
    'targets': [
      {
            'target_name': 'test',
            'type': 'executable',
            'sources': [
                'src/assets/assets.cc',
                'src/tests/main.cc',
                'src/world/world.cc',
                'src/world/track.cc',
                'src/world/world-object.cc',
		'src/gfx/openvg-companion.cc',
		'src/tests/test-assets.cc',
		'src/tests/test-common.cc',
		'src/tests/test-world.cc',
		'src/tests/test-openvg-companion.cc',
		'src/tests/test-tracks.cc',
		'src/tests/test-system-time.cc',
                'src/common/common.cc',
		'third-party/jsoncpp/jsoncpp.cpp',
                ],
            'cflags' : ["-g"],
            'include_dirs': [
                '.',
                'src',
		'third-party/jsoncpp',
		'third-party/eigen',
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
                          '/usr/local/opt/libpng/include',
                          '/usr/local/include',
                          "/opt/ShivaVG/include",
                          'osx-gmock-1.6.0/include',
                          'osx-gmock-1.6.0/gtest/include',
                          ],
                      'libraries' : [
                          '-L/usr/local/opt/libpng/lib',
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
                'src/gfx/openvg-companion.cc',
		'src/gfx/openvg-adapter-impl.cc',
                ],
            'include_dirs': [
                '.',
                'src',
		'third-party/jsoncpp',
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
                          "/opt/ShivaVG/include",
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
        {
            'target_name': 'mac_rracer',
            'product_name': 'MacRaspberryRacer',
            'type': 'executable',
            'cflags' : ["-g"],
            'mac_bundle': 1,
            'sources': [
		'src/gfx/openvg-companion.cc',
		'src/gfx/openvg-adapter-impl.cc',
		'src/mac/mac-window-adapter.cc',
                'src/assets/assets.cc',
                'src/mac/RaspRacerAppDelegate.h',
                'src/mac/RaspRacerAppDelegate.mm',
                'src/mac/RaspRacerView.h',
                'src/mac/RaspRacerView.mm',
                'src/mac/RRApplication.mm',
                'src/mac/main.m',
                'src/world/world.cc',
                'src/world/world-object.cc',
                'src/world/track.cc',
                'src/common/common.cc',
		'third-party/jsoncpp/jsoncpp.cpp',
                ],
            'libraries' : [
                '-L/usr/local/opt/libpng/lib',
                '-lpng15',
                '-L/usr/local/lib',
                '-lboost_filesystem-mt',
                '-lboost_system-mt',
                '-lpng15',
                ],

            'mac_bundle_resources': [
                'src/mac/Resources/InfoPlist.strings',
                'src/mac/Resources/MainMenu.xib',
		'resources',
                ],
            'link_settings': {
                'libraries': [
                    '$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
                    '$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
                    "/opt/ShivaVG/lib/libOpenVG.a",
                    ],
                },
            'include_dirs' : [
                "/opt/ShivaVG/include",
                '/usr/local/opt/libpng/include',
                '/usr/local/include',
		'third-party/jsoncpp',
		'third-party/eigen',
                "src",
                ],
            'xcode_settings': {
                'ARCHS': [ 'x86_64' ],
                'INFOPLIST_FILE': 'src/mac/RaspRacer-Info.plist',
                },
            },
        ], # targets
    }
