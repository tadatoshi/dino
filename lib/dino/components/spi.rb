module Dino
  module Components
    class Spi < BaseComponent

      def after_initialize(options={})
        set_pin_mode(:out)
      end

      def begin
        self.board.begin
      end

      def end
        self.board.end
      end

      def set_bit_order(bit_order)
        self.board.set_bit_order(bit_order)
      end

      def set_clock_divider(clock_divider)
        self.board.set_clock_divider(clock_divider)
      end

      def set_data_mode(data_mode)
        self.board.set_data_mode(data_mode)
      end

      def transfer(value)
        self.board.transfer(value)
      end

    end
  end
end