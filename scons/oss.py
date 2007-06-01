#
# oss.py
#
# Open Source Software builder
#

import os
import tarfile

import SCons.Action
import SCons.Builder

oss_debug = 0

def debug(level, str):
    if (oss_debug >= level):
        print 'OSS: ' + str

def url_get(proto, url, dldir):
    # XXX: need to implement this
    return False

def get_sources(node, env, path):
    '''Scanner for dependencies'''
    debug(1, 'get_sources %s' % node)
    patches = env.Dictionary().get('patches',[])
    path = env.Dictionary().get('builddir','')
    sources = []
    for i in patches:
        sources.append('%s/%s' % (path,i))
    return sources

def get_targets(node, env, path):
    '''Scanner for dependencies'''
    debug(1, 'get_targets %s' % node)
    patches = env.Dictionary().get('patches',[])
    targets = []
    return targets

def scan_source(node, env):
    '''Determine whether the node should be checked for dependencies'''
    debug(1, 'scan_source %s' % node)
    return True

def scan_target(node, env):
    '''Determine whether the node should be checked for dependencies'''
    debug(1, 'scan_target %s' % node)
    return True
    
def ossEmitter(target, source, env):
    '''Generate the tuple of target and source nodes'''
    debug(1, 'ossEmitter')
    cross = env.Dictionary().get('cross',[])
    dldir = env.Dictionary().get('dldir',[])
    slist = []
    for i in source:
        s = str(i)
        p = s.find(':')
        proto = s[:p]
        p = s.rfind('/') + 1
        file = s[p:]
        path = '%s/%s' % (dldir,file)
        if not os.path.exists(path):
            if proto == 'http':
                url_get(proto, s, dldir)
            elif proto == 'ftp':
                url_get(proto, s, dldir)
            else:
                raise 'Unknown URL protocol'
        slist = [ path ]
    return (target, slist)

def build_action(source, target, env):
    '''Perform the build'''
    debug(1, 'build_action')
    if len(source) != 1:
        raise 'Builder only supports a single source file.'

    buildtarget = env.Dictionary().get('buildtarget','')
    patches = env.Dictionary().get('patches',[])
    path = env.Dictionary().get('builddir','')
    cross = env.Dictionary().get('cross','')
    prefix = env.Dictionary().get('prefix','')
    cmd = env.Dictionary().get('command','')

    src = str(source[0])
    i = src.rfind('.') + 1
    ext = src[i:]
    tar = tarfile.open(src, 'r:%s' % ext)
    for i in tar:
        j = i.name.find('/')
        dir = '%s/%s/%s' % (path,buildtarget,i.name[:j])
        break
    if os.path.exists(dir):
        os.system('rm -rf %s' % dir)
    for i in tar:
        tar.extract(i,'%s/%s' % (path,buildtarget))
    tar.close()

    for i in patches:
        os.system('cd %s && patch -p1 < ../../%s' % (dir,i))

    cc = cross + 'gcc'
    command = 'CROSS_PREFIX=%s CC=%s INSTALL_PREFIX=%s sh -c "cd %s' % (cross,cc,prefix,dir)
    for i in cmd:
        command += ' && %s' % i
    command += '"'
    build = os.system(command)
    return None

def ossTarget(target):
    debug(1, 'ossTarget %s' % target)
    return SCons.Node.FS.default_fs.Entry(target)

def generate(env):
    '''
    Open Source Software builder
    '''
    scanner_s = env.Scanner(
        get_sources,
        "OSS Source Scan",
        scan_check = scan_source,
        )

    scanner_t = env.Scanner(
        get_targets,
        "OSS Target Scan",
        scan_check = scan_target,
        )

    oss_builder = SCons.Builder.Builder(
        action = build_action,
        emitter = ossEmitter,
        target_factory = ossTarget,
        single_source = False,
        source_scanner =  scanner_s,
        target_scanner =  scanner_t,
        )

    env.Append(BUILDERS = {
        'oss': oss_builder,
        })

    env.AppendUnique(
        OSS = 'oss',
        )

def exists(env):
    return True
