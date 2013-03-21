require 'spec_helper'

module Dino
  module Components
    describe Spi do
      let(:board) { mock(:board, analog_write: true, set_pin_mode: true) }

      describe '#initialize' do
        it 'should raise if it does not receive a pin' do
          expect {
            Spi.new(board: board)
          }.to raise_exception
        end

        it 'should raise if it does not receive a board' do
          expect {
            Spi.new(pin: 10)
          }.to raise_exception
        end

        it 'should set the SS (Slave Select) pin to out' do
          board.should_receive(:set_pin_mode).with(10, :out)
          Spi.new(pin: 10, board: board)
        end
      end

      describe '#begin' do
        let(:spi) {spi = Spi.new(pin: 10, board: board)}

        it 'should begin SPI communication' do
          board.should_receive(:begin)
          spi.begin
        end
      end

      describe '#end' do
        let(:spi) {spi = Spi.new(pin: 10, board: board)}

        it 'should end SPI communication' do
          board.should_receive(:end)
          spi.end
        end
      end

      describe '#set_bit_order' do
        let(:spi) {spi = Spi.new(pin: 10, board: board)}

        it 'should set bit order to LSBFIRST' do
          board.should_receive(:set_bit_order).with(Board::LSBFIRST)
          spi.set_bit_order(Board::LSBFIRST)
        end
      end 

      describe '#set_clock_divider' do
        let(:spi) {spi = Spi.new(pin: 10, board: board)}

        it 'should set clock divider to SPI_CLOCK_DIV2' do
          board.should_receive(:set_clock_divider).with(Dino::Boards::SpiMasterBoard::SPI_CLOCK_DIV2)
          spi.set_clock_divider(Dino::Boards::SpiMasterBoard::SPI_CLOCK_DIV2)
        end
      end  

      describe '#set_data_mode' do
        let(:spi) {spi = Spi.new(pin: 10, board: board)}

        it 'should set data mode to SPI_MODE0' do
          board.should_receive(:set_data_mode).with(Dino::Boards::SpiMasterBoard::SPI_MODE0)
          spi.set_data_mode(Dino::Boards::SpiMasterBoard::SPI_MODE0)
        end
      end 

      describe '#transfer' do
        let(:spi) {spi = Spi.new(pin: 10, board: board)}

        it 'should transfer a value to SPI slave device' do
          board.should_receive(:transfer).with(5)
          spi.transfer(5)
        end
      end                          
      
    end
  end
end