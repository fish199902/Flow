from os.path import join
import os

env = Environment(CPPPATH = ['.', './framework', './btogre', './states'],
                  LIBS = ['bulletdynamics', 'bulletcollision', 'bulletmath', 'OgreMain', 'OIS', 'CEGUIBase'])
env.Decider('MD5-timestamp')

if os.name == 'nt':
    env.AppendUnique(CPPPATH = [join("H:/App", path) for path in ['OgreSDK/samples/include',
                                                                  'OgreSDK/samples/refapp/include',
                                                                  'OgreSDK/include',
                                                                  'OgreSDK/include/CEGUI',
                                                                  'OgreSDK/include/OIS',
                                                                  'bullet-2.74/src']],
                     LIBPATH = [join("H:/App", path) for path in ['OgreSDK/lib',
                                                                  'OgreSDK/bin/Release',
                                                                  'bullet-2.74/out/release8/libs']],
                     LIBS = ['OgreGUIRenderer'])
else:
    env.AppendUnique(CPPPATH = ['/usr/local/include/OGRE',
                                '/usr/include/CEGUI',
                                '/usr/include/OIS',
                                '/home/fish/Flow/ogre/Samples/Common/include'],
                     LIBPATH = ['/home/fish/Flow/bullet-2.74/src/.libs',
                                '/home/fish/Flow/bullet-2.74/Extras'],
                     LIBS = ['CEGUIOgreRenderer'])

print 'Compiler:', env['CC']
print 'Libpath:', env['LIBPATH']
print 'Cpppath:', env['CPPPATH']

env.Program('flow', 'main.cpp')
