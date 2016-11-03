require 'RMagick'
include Magick

def main num, count
  animation = ImageList.new(*Dir["tmp*.jpg"])
  animation.delay = 100
  animation.ticks_per_second = 2
  animation.write("WANTED#{count}.gif")
  `rm tmp*.jpg 2> /dev/null`
end

main ARGV[0], ARGV[1]
