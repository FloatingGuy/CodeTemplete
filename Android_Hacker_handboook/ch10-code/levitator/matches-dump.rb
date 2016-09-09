#!/usr/bin/env ruby

#
# match leaked data to kallsyms
#
# -jduck
#

fn = 'dump'
fn = ARGV.shift if ARGV.length > 0

# read kallsyms, build hash of addresses to names
syms = {}
File.open('kallsyms', 'rb') { |f|
  f.each { |ln|
    next if ln !~ /^c[0-9a-f]{7}/
    ln.chomp!
    addr,type,name = ln.split
    addr = '0x' + addr
    syms[addr] ||= []
    syms[addr] << [ type, name ]
  }
}

# read the leaked data, extract offsets and values leaked
matched = {}
File.open(fn, 'rb') { |f|
  data = f.read
  data.unpack('V*').each_with_index { |v,idx|
    value = "0x%x" % v
    next if not syms[value]
    offset = "0x%x" % (idx * 4)
    matched[value] ||= []
    matched[value] << offset
  }
}

matched.each { |k,v|
  #puts "#{k} #{syms[k].join(' ')} - #{v.join(', ')}"
  puts "#{k} #{syms[k].join(' ')}" #  - #{v.join(', ')}"
}
