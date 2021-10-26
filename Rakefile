task default: %w[make]

make_task = task :make do
  ruby 'make.rb'
end

task :run => make_task do
  system 'qemu-system-i386 -cdrom epsilon.iso'
end

task :clean do
  system 'rm -rfv builddir'
end

retouch_task = task :retouch do
  system 'fd -e c -e s -x touch'
end

task all: [retouch_task, make_task]
