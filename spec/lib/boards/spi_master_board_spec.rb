require 'spec_helper'

module Dino
  describe Dino::Boards::SpiMasterBoard do
    def io_mock(methods = {})
      @io ||= mock(:io, {write: nil, add_observer: nil}.merge(methods))
    end

    subject { Boards::SpiMasterBoard.new(io_mock) }

    describe '#begin' do
      it 'should begin SPI communication' do
        io_mock.should_receive(:write).with('!9600.')
        subject.begin
      end
    end

    describe '#end' do
      it 'should end SPI communication' do
        io_mock.should_receive(:write).with('!9601.')
        subject.end
      end
    end

    describe '#set_bit_order' do
      it 'should set bit order for SPI communication' do
        io_mock.should_receive(:write).with('!9602000.')
        subject.set_bit_order(Board::LSBFIRST)
      end
    end

    describe '#set_clock_divider' do
      it 'should set clock divider for SPI communication' do
        io_mock.should_receive(:write).with('!9603002.')
        subject.set_clock_divider(Dino::Boards::SpiMasterBoard::SPI_CLOCK_DIV2)
      end
    end 

    describe '#set_data_mode' do
      it 'should set data mode for SPI communication' do
        io_mock.should_receive(:write).with('!9604000.')
        subject.set_data_mode(Dino::Boards::SpiMasterBoard::SPI_MODE0)
      end
    end        

    describe '#transfer' do
      it 'should transfer a value to SPI slave device' do
        io_mock.should_receive(:write).with('!9605005.')
        subject.transfer(5)
      end
    end        

    describe '#normalize_spi_method_indicator' do
      it 'should normalize numbers so they are two digits' do
        subject.normalize_spi_method_indicator(1).should == '01'
      end

      it 'should not normalize numbers that are already two digits' do
        subject.normalize_spi_method_indicator(10).should == '10'
      end

      it 'should raise if a number larger than two digits are given' do
        expect { subject.normalize_spi_method_indicator(1000) }.to raise_exception 'SPI method indicator can only be two digits'
      end
    end

  end
end
