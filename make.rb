require 'colorize'

COMPILER = ENV['CC'] || 'i686-elf-gcc'
ASSEMBLER = ENV['ASM'] || 'i686-elf-as'
LINKER = ENV['LD'] || 'i686-elf-ld'

SOURCES = File.read('c.list').split
ASM = File.read('s.list').split
BUILD = 'builddir'
KERNEL = 'kernel'
FULL_KERNEL = "#{BUILD}/#{KERNEL}"

class String
  def to_obj
    "#{BUILD}/#{self}.o"
  end
end

OBJS = SOURCES.map(&:to_obj) + ASM.map(&:to_obj)

ok = true

if !File.directory?(BUILD)
  ok = system "mkdir #{BUILD}"
end

exit 1 unless ok

# build C code
SOURCES.each do |src|
  puts '%15s : %30s => %-30s' % [COMPILER, src.blue, src.to_obj.green]
  ok = system "#{COMPILER} -c #{src} -o #{src.to_obj} -ffreestanding"
  exit 1 unless ok
end
# build ASM code
ASM.each do |asm|
  puts '%15s : %30s => %-30s' % [ASSEMBLER, asm.blue, asm.to_obj.green]
  ok = system "#{ASSEMBLER} #{asm} -o #{asm.to_obj}"
  exit 1 unless ok
end

# link
puts "%15s : [#{OBJS.map(&:blue).join(', ')}] => #{FULL_KERNEL.green}" % [LINKER]
ok = system "#{LINKER} -T link.ld -nostdlib #{OBJS.join(' ')} -o #{FULL_KERNEL}"
exit 1 unless ok

# check bootable
ok = system "grub-file --is-x86-multiboot #{FULL_KERNEL}"
exit 1 unless ok
