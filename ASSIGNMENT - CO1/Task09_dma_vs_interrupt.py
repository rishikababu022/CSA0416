#!/usr/bin/env python3
"""Task 9: interrupt-driven I/O (one interrupt per word) vs DMA for a 500 MB MRI image.
Edit the constants to match the assumptions stated in your report."""
IMAGE = 500 * 1024 * 1024        # bytes
WORD = 4                         # bytes per word (32-bit)
IRQ_COST = 2e-6                  # seconds of CPU per interrupt (assumed 2 us)
DMA_BLOCK = 64 * 1024            # bytes per DMA transfer (one interrupt per block)
DMA_SETUP = 5e-6                 # CPU seconds to program the DMA controller per block
words = IMAGE // WORD
blocks = IMAGE // DMA_BLOCK
print(f"Interrupt-driven: {words:,} interrupts, CPU time = {words*IRQ_COST:.2f} s, buffer = {WORD} B (one word)")
print(f"DMA             : {blocks:,} interrupts, CPU time = {blocks*(IRQ_COST+DMA_SETUP):.4f} s, buffer = {DMA_BLOCK//1024} KB (per block)")
print(f"Ratio of interrupts: {words//blocks}x fewer with DMA")
