import os.path
from SCons.Script import *

def parseLogOption(value):
    stream, area, level = ( x.strip() for x in value.strip().split('|') )
    stream = ''.join('(%s)' % x for x in stream.split('::') )
    if area : area = ''.join( '(%s)' % x for x in area.split('::') )
    else    : area = '(_)'
    return '((%s,%s,%s))' % (stream,area,level)

def expandLogOption(target, source, env, for_signature):
    if env.get('LOGLEVELS'):
        return [ 'SENF_LOG_CONF="' + ''.join( parseLogOption(x) for x in env.subst('$LOGLEVELS').split() )+'"']
    else:
        return []

class BuildTypeOptions:
    def __init__(self, var):
        self._var = var

    def __call__(self, target, source, env, for_signature):
        type = env['final'] and "final" or env['debug'] and "debug" or "normal"
        return env[self._var + "_" + type]

###########################################################################
# This looks much more complicated than it is: We do three things here:
# a) switch between final or debug options
# b) parse the LOGLEVELS parameter into the correct SENF_LOG_CONF syntax
# c) check for a local SENF, set options accordingly and update that SENF if needed

def SetupForSENF(env, senf_paths = []):
    senf_paths.extend(('senf', '../senf', os.path.dirname(os.path.dirname(__file__)),
                       '/usr/local', '/usr'))
    env.Append(
        LIBS              = [ 'senf', 'rt', '$BOOSTREGEXLIB',
                              '$BOOSTIOSTREAMSLIB', '$BOOSTSIGNALSLIB',
                              '$BOOSTFSLIB' ],
        BOOSTREGEXLIB     = 'boost_regex',
        BOOSTIOSTREAMSLIB = 'boost_iostreams',
        BOOSTSIGNALSLIB   = 'boost_signals',
        BOOSTFSLIB        = 'boost_filesystem',
        
        CXXFLAGS          = [ '-Wno-long-long', '$CXXFLAGS_' ],
        CXXFLAGS_         = BuildTypeOptions('CXXFLAGS'),
        
        CPPDEFINES        = [ '$expandLogOption', '$CPPDEFINES_' ],
        expandLogOption   = expandLogOption,
        CPPDEFINES_       = BuildTypeOptions('CPPDEFINES'),
        
        LINKFLAGS         = [ '-rdynamic', '$LINKFLAGS_' ],
        LINKFLAGS_        = BuildTypeOptions('LINKFLAGS'),

        LOGLEVELS         = [ '$LOGLEVELS_' ],
        LOGLEVELS_        = BuildTypeOptions('LOGLEVELS'),
        )

    env.SetDefault( 
        CXXFLAGS_final    = [],
        CXXFLAGS_normal   = [],
        CXXFLAGS_debug    = [],

        CPPDEFINES_final  = [],
        CPPDEFINES_normal = [],
        CPPDEFINES_debug  = [],

        LINKFLAGS_final   = [],
        LINKFLAGS_normal  = [],
        LINKFLAGS_debug   = [],

        LOGLEVELS_final   = [],
        LOGLEVELS_normal  = [],
        LOGLEVELS_debug   = [],
        )

    # Interpret command line options
    opts = Variables(args=ARGUMENTS)
    opts.Add( 'LOGLEVELS', 'Special log levels. Syntax: <stream>|[<area>]|<level> ...',
              '${"$LOGLEVELS_"+(final and "final" or "debug")}' )
    opts.Add( BoolVariable('final', 'Build final (optimized) build', False) )
    opts.Add( BoolVariable('debug', 'Link in debug symbols', False) )
    opts.Update(env)
    env.Replace(**dict(opts.UnknownVariables()))
    env.Help(opts.GenerateHelpText(env))

    # If we have a symbolic link (or directory) 'senf', we use it as our
    # senf repository
    for path in senf_paths:
        if not path.startswith('/') : sconspath = '#/%s' % path
        else                        : sconspath = path
        if os.path.exists(os.path.join(path,"senf/config.hh")):
            print "\nUsing SENF in '%s'\n" \
                % ('/..' in sconspath and os.path.abspath(path) or sconspath)
            env.Append( LIBPATH = [ sconspath ],
                        CPPPATH = [ sconspath ],
                        BUNDLEDIR = sconspath )
            try:
                env.MergeFlags(file(os.path.join(path,"senf.conf")).read())
            except IOError:
                print "(SENF configuration file 'senf.conf' not found, assuming non-final SENF)"
                env.Append(CPPDEFINES = [ 'SENF_DEBUG' ])
            break
        elif os.path.exists(os.path.join(path,"include/senf/config.hh")):
            print "\nUsing system SENF in '%s/'\n" % sconspath
            env.Append(BUNDLEDIR = os.path.join(sconspath,"lib/senf"))
            break
    else:
        print "\nSENF library not found .. trying build anyway !!\n"


def DefaultOptions(env):
    env.Append(
        CXXFLAGS         = [ '-Wall', '-Woverloaded-virtual' ],
        CXXFLAGS_final   = [ '-O2' ],
        CXXFLAGS_normal  = [ '-O0', '-g' ],
        CXXFLAGS_debug   = [ '$CXXFLAGS_normal' ],

        LINKFLAGS_normal = [ '-Wl,-S' ],
    )