task default: %w[make]

make_task = task :make do
  ruby 'make.rb'
end

task :run => make_task do
  system 'qemu-system-i386 -cdrom alpha.iso'
end
