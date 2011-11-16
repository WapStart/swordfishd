import os

env = Environment()
env['CCFLAGS'] = '-O3'
AddOption('--extrapath',
          dest='extrapath',
          type='string',
          nargs=1,
          action='store',
          help='comma separated list of add''l paths  (--extrapath /opt/foo/,/foo) static linking')

def add_extra_libs(s):
  for x in s.split(','):
    if os.path.exists(x):
      env.Append(CPPPATH=[x + '/include'])
      env.Append(LIBPATH=[x + '/lib'])
      env.Append(LIBPATH=[x + '/lib64'])

if(GetOption('extrapath')):
  add_extra_libs(GetOption('extrapath'))

add_extra_libs('/usr/local')

env.Append(CPPPATH=['include'])

boost_libs = ['program_options', 'system', 'thread']

cfg = env.Configure()
for lib in boost_libs:
  if not cfg.CheckLib("boost_%s" % lib):
    print "scons: %s boost library not found" % lib
    Exit(-1)

SConscript('SConscript', exports='env')
SConscript('tests/SConscript', exports='env')
