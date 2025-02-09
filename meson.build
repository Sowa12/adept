project('adept', 'cpp')

bin_name = 'adept'

inc = include_directories('include')

source = files('src/main.cc',
        'src/context.cc',
        'src/group_state.cc',
        'src/info_state.cc',
        'src/read_state.cc')

executable(bin_name, 
        source,
        include_directories: inc)