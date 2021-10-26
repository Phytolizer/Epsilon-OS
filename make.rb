require "colorize"
require "date"
require "pathname"

COMPILER = ENV["CC"] || "i686-elf-gcc"
ASSEMBLER = ENV["ASM"] || "i686-elf-as"
LINKER = ENV["LD"] || "i686-elf-ld"

exe_length = [COMPILER.length, ASSEMBLER.length, LINKER.length].max + "-- ".length

SOURCES = File.read("c.list").split
ASM = File.read("s.list").split
BUILD = "builddir"
KERNEL = "kernel"
NAME = "epsilon"
FULL_KERNEL = "#{BUILD}/#{KERNEL}"

class String
  def to_obj
    p = Pathname(self).each_filename.to_a
    "#{BUILD}/#{p.join("_")}.o"
  end
end

OBJS = SOURCES.map(&:to_obj) + ASM.map(&:to_obj)

def run_command(cmd, fatal: true)
  ok = system cmd
  exit 1 unless ok || !fatal
  ok
end

if !File.directory?(BUILD)
  run_command "mkdir #{BUILD}"
end

should_rebuild = {}

(SOURCES + ASM).each do |f|
  obj = f.to_obj
  mtime = File.mtime(f)
  mtime = DateTime.parse(mtime.to_s)
  stamp = "#{obj}.stamp"
  last_mtime = File.exist?(stamp) ? File.read(stamp) : nil
  last_mtime = DateTime.iso8601(last_mtime) unless last_mtime.nil?
  should_rebuild[obj] = last_mtime.nil? || last_mtime.iso8601 < mtime.iso8601
end

puts "-- Build #{"C".yellow} code"
SOURCES.each do |src|
  next unless should_rebuild[src.to_obj]
  puts "%#{exe_length}s : %s => %s" % [COMPILER, src.blue, src.to_obj.green]
  run_command "#{COMPILER} -c #{src} -o #{src.to_obj} -ffreestanding"
end
puts "-- Build #{"assembly".yellow} code"
ASM.each do |asm|
  next unless should_rebuild[asm.to_obj]
  puts "%#{exe_length}s : %s => %s" % [ASSEMBLER, asm.blue, asm.to_obj.green]
  run_command "#{ASSEMBLER} #{asm} -o #{asm.to_obj}"
end

if should_rebuild.any? { |_, v| v }
  puts "-- #{"Link".yellow}"
  puts "%#{exe_length}s : [#{OBJS.map(&:blue).join(", ")}] => #{FULL_KERNEL.green}" % [LINKER]
  run_command "#{LINKER} -T link.ld -nostdlib #{OBJS.join(" ")} -o #{FULL_KERNEL}"

  # check bootable
  print "-- #{"Check".yellow} image..."
  ok = run_command "grub-file --is-x86-multiboot #{FULL_KERNEL}", fatal: false
  puts ok ? "OK".green : "NOT OK".red
  exit 1 unless ok

  puts "-- #{"Pack".yellow} image into ISO"
  run_command "mkdir -p isodir/boot/grub"
  run_command "cp #{FULL_KERNEL} isodir/boot/#{NAME}.bin"
  run_command "cp grub.cfg isodir/boot/grub/grub.cfg"
  run_command "grub-mkrescue -o #{NAME}.iso isodir"
end

puts "-- #{"SUCCESS".green}"

OBJS.each do |obj|
  File.write("#{obj}.stamp", DateTime.parse(File.mtime(obj).to_s).iso8601)
end
