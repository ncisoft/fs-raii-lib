import os
import sys
import SCons.Builder
import SCons.Action
import platform
from pyjavaproperties import Properties
from inspect import getmembers
from pprint import pprint


props = Properties()
fpath = 'lua.scons.properties'
if os.path.exists(fpath):
    props.load(open(fpath))
print(props['LUA'])


def complain_ctags(target, source, env):
    print('INFORMATION: ctags binary was not found (see above). Tags have not been built.')


def generate(env):
    env['CTAGS'] = find_ctags(env)
    if env['CTAGS'] is not None:
        # env['CTAGSCOM'] = 'cd $TARGET.dir; ctags -R .'
        customized = ('$CTAGS --tag-relative=yes --langmap=c:.c.h -f '
                      '$TARGET src/*.[hc] tests/*.[hc] .xopt/include/*.h')
        env['CTAGSCOM'] = customized
        env['BUILDERS']['ctags'] = SCons.Builder.Builder(action=customized)
    else:
        env['BUILDERS']['ctags'] = SCons.Builder.Builder(action=env.Action(complain_ctags))

def find_ctags(env):
    b=env.WhereIs('ctags-exuberant')
    if b == None:
        print('Searching for ctags: not found. Tags will not be built')
    else:
#        b = '/usr/bin/ctags'
        print('Searching for ctags: ', b)
    return b

def exists(env):
    if find_ctags(env) == None:
        return 0
    return 1

home_dir = os.environ['HOME']
env = Environment(CC = 'clang',
                  CCFLAGS=['-g', '-std=gnu99', '-Wall', '-Wno-unused-label',
                           '-O2',
                           '-I./include',
                           '-I./.xopt/include',
                           '-I./',
                           ])
env["CC"] = os.getenv("CC") or env["CC"]
env["CXX"] = os.getenv("CXX") or env["CXX"]
env["ENV"].update(x for x in os.environ.items() if x[0].startswith("C"))

def CheckPKGConfig(context, version):
    context.Message( 'Checking for pkg-config... ' )
    ret = context.TryAction('pkg-config --atleast-pkgconfig-version=%s' % version)[0]
    context.Result( ret )
    return ret

def CheckPKG(context, name):
    context.Message( 'Checking for %s... ' % name )
    ret = context.TryAction('pkg-config --exists \'%s\'' % name)[0]
    context.Result( ret )
    return ret

lua=''
libs= 'pthread'

def CheckOS():
    global lua
    global libs
    os = platform.system()
    if os == "Darwin":
        lua = "lua5.1"
        libs = ['lua.5.1', 'm', 'pthread']
    elif os == "Linux":
        lua = "lua-5.1"
        libs = 'pthread'
    else:
        print("unsupport OS: " + os)
        sys.exit(1)

conf = Configure(env, custom_tests = { 'CheckPKGConfig': CheckPKGConfig,
                                      'CheckPKG': CheckPKG })

#if not conf.CheckPKG('gtk+-2.0 >= 2.4.9'):

CheckOS()
if not conf.CheckPKG('libcork'):
    print('check not found.')
    sys.exit(1)
# print(env.__dict__)
# print(env['CPPPATH'])
# print(env['LIBS'])


# if ARGUMENTS.get("LUA") == None:
#    env.ParseConfig('pkg-config --cflags --libs ' + lua)
# elif ARGUMENTS.get("LUA") == 'lua52':
#     env.ParseConfig('echo -I/opt/lua52/include/ -L/opt/lua52/lib/ -llua')
# elif ARGUMENTS.get("LUA") == 'lua54':
#    env.ParseConfig('echo -I/opt/lua54/include/ -L/opt/lua54/lib/ -llua')
# env.ParseConfig('pkg-config --cflags --libs libcork')
env.ParseConfig('echo ./.xopt/lib/libcork.a')
env.ParseConfig('echo -lev')
env.Tool('compilation_db')
cdb = env.CompilationDatabase('compile_database.json')

generate(env)


lib_logger = File('.xopt/lib/liblogger.a')
xlibs = [libs, lib_logger]
os = platform.system()
print(Dir('.').abspath)
print(Dir('#').abspath)
top_dir = Dir('.').abspath

header_deps = ['include/utils.h', 'include/fs_raii.h']
for i, dep in enumerate(header_deps):
    header_deps[i] = top_dir + "/" + dep

env["header_deps"] = header_deps
Export('env')
Alias('tags', env.ctags(source='src/fs_raii.c', target='tags'))
Alias('cdb', cdb)

SConscript('src/SConstruct', variant_dir='build/core', exports='env')
SConscript('tests/SConstruct', exports='env')
