module Dino
  module Boards
    class SpiMasterBoard < Board

      SPI_CLOCK_DIV2 = 2
      SPI_CLOCK_DIV4 = 4
      SPI_CLOCK_DIV8 = 8
      SPI_CLOCK_DIV16 = 16
      SPI_CLOCK_DIV32 = 32
      SPI_CLOCK_DIV64 = 64
      SPI_CLOCK_DIV128 = 128

      SPI_MODE0 = 0
      SPI_MODE1 = 1
      SPI_MODE2 = 2
      SPI_MODE3 = 3             

      def begin
        spi_method_indicator = normalize_spi_method_indicator(0)
        write("96#{spi_method_indicator}")
      end

      def end
        spi_method_indicator = normalize_spi_method_indicator(1)
        write("96#{spi_method_indicator}")        
      end

      def set_bit_order(bit_order)
        spi_method_indicator = normalize_spi_method_indicator(2)
        bit_order = normalize_value(bit_order)
        write("96#{spi_method_indicator}#{bit_order}")         
      end 

      def set_clock_divider(clock_divider)
        spi_method_indicator = normalize_spi_method_indicator(3)
        clock_divider = normalize_value(clock_divider)
        write("96#{spi_method_indicator}#{clock_divider}") 
      end

      def set_data_mode(data_mode)
        spi_method_indicator = normalize_spi_method_indicator(4)
        data_mode = normalize_value(data_mode)
        write("96#{spi_method_indicator}#{data_mode}") 
      end

      def transfer(value)
        spi_method_indicator = normalize_spi_method_indicator(5)
        value = normalize_value(value)
        write("96#{spi_method_indicator}#{value}") 
      end

      def normalize_spi_method_indicator(spi_method_indicator)
        raise Exception.new('SPI method indicator can only be two digits') if spi_method_indicator.to_s.length > 2
        normalize(spi_method_indicator, 2)
      end

    end
  end
end