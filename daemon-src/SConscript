Import('env')

boost_libs = ['program_options']

#cfg = env.Configure()
#for lib in boost_libs:
#  if not cfg.CheckLib("boost_%s" % lib):
#    print "scons: %s boost library not found" % lib
#    Exit(-1)

env.Program('swordfish', [Glob('src/*.cpp')])
