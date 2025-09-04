# Instruction Set Architecture:
* ISA is a layer of abstraction that softwares use when utilizing underlying hardware. Based on the layer of hierarchy of abstraction of computer. From low to high abstraction: 
Electronics and Physics -> IC's and Transistors -> Logic Gates, Registers -> Micro Architecture -> Instruction Set Architecture -> Machine Code -> Assembly Language -> Programming Language -> Algorithm -> Application.
* Formally, the ISA defines the various instructions, registers and memory addressing modes that the processor would support, at a hardware level. Software created to run on an ISA would be designed to use the ISA's instructions, register and memory addressing modes to carry out its tasks. 
* Designing an ISA doesn't just involve deciding what set of instructions to support, what registers to use and what memory addressing modes to use. 
-  When coming up with the set of instructions (and the corresponding machine codes for each instruction), one must consider how the hardware would be designed to support these instructions. Instructions are ultimately represented in machine code, which is a string of 0s and 1s. The underlying hardware should be designed to read these string of 0s and 1s, and deduce which type of instruction it is, the corresponding operands, and should accordingly decide which block of the CPU should handle this instruction. This complex process is known as "decoding", and is a fundamental part of modern processors. Here's and example:
+ Say we have a RISC-V CPU that reads instructions 00000000001100010000000010110011. This value 0x003100c3 in hexadecimal. The CPU's decoder should be able to read this 32-bit instruction, figure out that this is an "add" instruction, the destination register is x1, and the source operands are registers x2 and x3. Thus, the instruction would be: add x1, x2, x3 
+ The RISC-V ISA is designed in such a way, the destination register is always encoded in the exact same position of every 32-bit instruction. The source operand(s) are also encoded in the exact same position of the instruction. This would ensure that when the decoder reads the source and/or destination registers, it would always read from the same position, hence there wouldn't have to be extra hardware logic to figure out which part of the instruction has the register(s) encoded. Lesser hardware logic would, by default, lead to lesser power consumption.
+ Continuing with the previous example, if you look up the RISC-V definition of the add instruction, you'll see that there's 5 bits to encode one register (source and/or destination). This would imply that there's a maximum of 2^5 = 32 registers (commonly known as architectural registers) available as part of the ISA. Increasing or decreasing the number of architectural registers would mean more/less bits to encode one register, which would increase/decrease the size of the instruction. 
+ Increasing/decreasing the number of instructions part of the ISA would also increase/decrease the instruction size. 

# Complex Instruction Set Computer (CISC): 
+ From a technical standpoint, CISC is used to describe ISAs where a single instruction is capable of executing more than one underlying operation in the CPU (typically known as micro-op or microcode).
+ A good example is the REPNE MOVSB instruction in x86. This is a single instruction in x86 that repeatedly copies one byte of data from source address (specified in DS register) to destination address (specified in DI register). This process involves multiple operations, such as; obtaining the byte of data from source address, writing the byte of data into destination address, incrementing source address, incrementing destination address, and repeating this operation as long as the value of register CX is non-zero. Each of the above operations would be single micro-ops in the 8086 CPU, but would be performed all at once when this one x86 instructions is executed.
+ Given the complexity of CISC instructions, different CISC instructions have different instruction lengths. Taking the same x86 ISA as an example, the INC instruction is only 1 byte long (this instruction just increases the value of a register by 1), whereas the instruction MOV [BX+DI+1234H], 5678H takes up 6 bytes (this instruction first obtains the address by adding the contents of BX and DI, then adding 0x1234 to that value, then writes value 0x5678 to that address). Having different instruction lengths allows for more compact code size (since simpler instructions have smaller lengths, as well as using complex instructions to replace multiple simpler instructions). However, the downsize is that the decoder for such an ISA would be rather complex. The decoder will have to implement logic to determine which bytes is the final byte of an instruction, which is easier said than done.
 
# Reduced Instruction Set Computer (RISC):
- Given the complexity of CISC, there was an imperative need to design a simpler ISA. Hence, many RISC ISAs came forward. RISC ISAs have instructions that execute as less micro-ops as possible (1 at the least). This ensured that instructions have a fixed length, meaning that the decode logic would be much simpler. On the flip side, this would increase the number of instructions of the program executed. 
-  The MIPS ISA is one of the most well-known RISC ISAs, and is widely taught in many universities as part of their Computer Architecture courses, since MIPS is regarded as a blueprint for designing efficient RISC ISAs. MIPS instructions are 32 bits long (4 bytes) and are divided into 3 types of instructions:
 ## R-type instructions: R-type instructions (R for register) are instructions where all the source operand(s) are registers.
 ## I-type instructions: I-type instructions (I for immediate values) are instructions where one of the operands is a constant value that is encoded in the instruction bits itself (immediate).
 ## J-type instructions: J-type instructions (J for jump) are instructions that change the flow of execution of the program, and are used to implement conditional logic.
- MIPS follows a register addressing mode, where the address is stored in a register. Any value to be written to/read from memory has the corresponding address stored in a register. Load instructions read values from memory and store instructions write values to memory. The simplicity of the addressing mode in MIPS also adds to the simpler hardware implementation of the MIPS ISA.
- RISC vs CISC has been an age-old debate. However, the bifurcation between RISC and CISC has slowly faded over time, with the introduction of extenstions to RISC ISAs such as ARM and RISC-V.
 
# Comparison of RISC and CISC:
## RISC (Reduced Instruction Set Computer)
### Strong Points 
* Simple instructions → smaller instruction set, easier to decode.
* Fixed instruction format → faster instruction execution and easier pipelining.
* High performance with pipelining → more instructions per clock cycle.
* Low power consumption → good for mobile and embedded devices.
* Compiler-friendly → optimized for modern compilers to generate efficient code.
### Drawbacks (Disadvantages):
* Requires more RAM → since complex tasks need multiple simple instructions.
* Larger code size → more instructions needed for the same task compared to CISC.
* Performance depends heavily on compiler optimization.

## CISC (Complex Instruction Set Computer)
### Strong Points: 
* Rich instruction set → can perform complex tasks with fewer instructions.
* Compact code size → smaller programs, saves memory.
* Efficient for assembly-level programming (programmers can use powerful instructions).
* Backward compatibility → many modern CPUs (like Intel x86) still run old code.
### Drawbacks (Disadvantages):
* Complex instruction decoding → harder to implement in hardware, slower clock cycles.
* Variable instruction length → makes pipelining more difficult.
* Higher power consumption compared to RISC.
* Less efficient at parallel execution (superscalar, pipelining) than RISC.

|             | RISC                                                                 | CISC                                                                 |
|-------------|----------------------------------------------------------------------|----------------------------------------------------------------------|
| Similarity  | Acts as an interface between machine code (software and microarchitecture) |
| Difference  | - RISC is a reduced instruction set                                  | - CISC is a complex instruction set                                  |
|             | - The number of instructions is less as compared to CISC             | - The number of instructions is more as compared to RISC             |
|             | - The addressing modes are fewer                                     | - The addressing modes are more                                      |
|             | - It works in a fixed instruction format                             | - It works in a variable instruction format                          |
|             | - The RISC consumes low power                                        | - The CISC consumes high power                                       |
|             | - The RISC processors are highly pipelined                           | - The CISC processors are less pipelined                             |
|             | - Optimizes performance by focusing on software                      | - Optimizes performance by focusing on hardware                      |
|             | - Requires more RAM                                                  | - Requires less RAM                                                   |
|             | - Simple hardware design                                             | - Complex hardware design                                             |
|             | - Used where low power, high efficiency, and scalability are critical (Mobile phones & tablets, Embedded systems, Networking devices, etc.) | - Used where complex computing is important (PCs & Laptops, Servers & Data centers, High-end workstations, etc.) |

# Personal thoughts:
I believe that in today’s world, RISC architectures are becoming increasingly suitable for embedded systems due to their high efficiency in power consumption and the simplicity of their hardware design. Since embedded devices often operate on limited battery resources, the low power requirements of RISC processors make them ideal for ensuring longer device lifespans. Moreover, the simpler hardware not only reduces manufacturing costs but also makes the processors more reliable and easier to integrate into compact systems.