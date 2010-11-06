if ARGV.size != 2
	puts "Usage: width height"
	exit
end

require 'benchmark'
include Benchmark

class Point
	def initialize(x, y)
		@x = x
		@y = y
	end

	def x
		@x
	end

	def y
		@y
	end

	def to_s
		"[" + x.to_s.rjust(2) + "," + y.to_s.rjust(2) + "]"
	end

	def up
		Point.new(@x, @y - 1)
	end

	def down
		Point.new(@x, @y + 1)
	end

	def right
		Point.new(@x + 1, @y)
	end

	def left
		Point.new(@x - 1, @y)
	end
end

class Array2D < Array
	def initialize(width, height, value = nil, nilvalue = value)
		@width = width
		@height = height
		@nilvalue = nilvalue
		super(width * height, value)
	end

	def [](point)
		if(point.class == Point)
			if (Range.new(0, @width - 1) === point.x) and (Range.new(0, @height - 1) === point.y)
				super(point.x + point.y * @width)
			else
				@nilvalue
			end
		else
			super(point)
		end
	end

	def []=(point, value)
		if(point.class == Point)
			if (Range.new(0, @width - 1) === point.x) and (Range.new(0, @height - 1) === point.y)
				super(point.x + point.y * @width, value)
			else
				throw Exception.new "Wrong index"
			end
		else
			super(point, value)
		end
	end

	def width
		@width
	end

	def height
		@height
	end

	def print
		str = String.new
		@height.times do |y|
			@width.times do |x|
				str += yield self[Point.new(x, y)]
			end
			str += "\n"
		end
		str
	end
end

class Map
	def initialize(width = 0, height = 0, array = nil)
		@width = width
		@height = height
		@array = nil
		@center = nil
		if ! array.nil?
			@array = array.clone
			return
		end
		@array = Array2D.new(width, height, false, true)

		#make list cells
		cells = Array.new
		width.times do |x|
			height.times do |y|
				cells << Point.new(x, y)
			end
		end

		#make
		neighbours = Array2D.new(width, height, -1)
		width.times do |x|
			height.times do |y|
				point = Point.new(x, y)
				xn = if (point.x == 0) || (point.x == width - 1)
					1
				else
					2
				end
	
				yn = if (point.y == 0) || (point.y == width - 1)
					1
				else
					2
				end

				neighbours[point] = xn + yn
			end
		end

		yfree = Array.new(height, width)
		xfree = Array.new(width, height)

		cells.shuffle!

		cells.delete_if { |cell| in_center(cell) }

		cells.each do |point|
			ok = true
			neighbours_range = Range.new(0, 2)

			if in_center(point) #reserve cell for individual
				ok = false
			elsif xfree[point.x] <= 1 # each row must have at least one free cell
				ok = false
			elsif yfree[point.y] <= 1 # each column must have at least one free cell
				ok = false
			elsif @array[point.up] && @array[point.right] && @array[point.right.up] # check for 2x2 blocks
				ok = false
			elsif @array[point.right] && @array[point.down] && @array[point.down.right] # check for 2x2 blocks
				ok = false
			elsif @array[point.down] && @array[point.left] && @array[point.left.down] # check for 2x2 blocks
				ok = false
			elsif @array[point.left] && @array[point.up] && @array[point.up.left] # check for 2x2 blocks
				ok = false
			elsif neighbours_range === neighbours[point.up] # check for deadend from up
				ok = false
			elsif neighbours_range === neighbours[point.right] # check for deadend from right
				ok = false
			elsif neighbours_range === neighbours[point.down] # check for deadend from down
				ok = false
			elsif neighbours_range === neighbours[point.left] # check for deadend from left
				ok = false
			else
				temp_map = @array.clone
				temp_map[point] = true
				if check_map(temp_map)
					@array[point] = true
					xfree[point.x] = xfree[point.x] - 1
					yfree[point.y] = yfree[point.y] - 1
					neighbours[point] = -1
					begin
						neighbours[point.up] = neighbours[point.up] - 1
					rescue
					end
					begin
						neighbours[point.down] = neighbours[point.down] - 1
					rescue
					end
					begin
						neighbours[point.left] = neighbours[point.left] - 1
					rescue
					end
					begin
						neighbours[point.right] = neighbours[point.right] - 1
					rescue
					end
				end
			end
		end
		check_map(@array)
	end

	# check accessible for each cell by wave algorithm
	def check_map(map)
		ways = Array2D.new(@width, @height, -1)
		@width.times do |x|
			@height.times do |y|
				point = Point.new(x, y)
				ways[point] = 0 if ! map[point]
			end
		end
		ways[@center] = 1

		new_cells = true
		step = 2
		while new_cells
			new_cells = false
			@width.times do |x|
				@height.times do |y|
					point = Point.new(x, y)
					if map[point]
						ways[point] = -1
					elsif ways[point] == 0
						if ways[point.up] == step - 1
							ways[point] = step
							new_cells = true
						end
						if ways[point.down] == step - 1
							ways[point] = step
							new_cells = true
						end
						if ways[point.left] == step - 1
							ways[point] = step
							new_cells = true
						end
						if ways[point.right] == step - 1
							ways[point] = step
							new_cells = true
						end
					end
				end
			end
			step += 1
		end

		@width.times do |x|
			@height.times do |y|
				point = Point.new(x, y)
				return false if ways[point] == 0
			end
		end
		true
	end

	# get center cell to place individual
	def in_center(point)
		if @center.nil?
			if
				if @width % 2 == 1
					(point.x == (@width - 1) / 2)
				else
					(Range.new(@width / 2 - 1, @width / 2) === point.x)
				end

				if
					if @height % 2 == 1
						(point.y == (@height - 1) / 2) 
					else
						(Range.new(@height / 2 - 1, @height / 2) === point.y)
					end

					@center = point
					true
				else
					false
				end
			else
				false
			end
		else
			false
		end
	end

	def to_s
		str = String.new
		str += "Width: " + @width.to_s + "\n"
		str += "Height: " + @height.to_s + "\n"
		str += "#" * (@width + 2) + "\n"
		@height.times do |y|
			str += "#"
			@width.times do |x|
				if (x == @center.x) && (y == @center.y)
					str += "@"
				else
					if @array[Point.new(x, y)]
						str += "#"
					else
						str += "."
					end
				end
			end
			str += "#\n"
		end
		str += "#" * (@width + 2) + "\n"
		str
	end

	def array
		@array
	end

	def center
		@center
	end
end

width = ARGV[0].to_i
height = ARGV[1].to_i

map = nil
bm(6) do |x|
	x.report("map") do map = Map.new(width, height)
	end
end

puts map

