#include "MethodTree.h"
#include "Insn.h"
#include "Opcodes.h"

TupulMethod* finishMethod(MethodTree tree, TupulClass* clazz) {
	TupulMethod* method = (TupulMethod*) calloc(sizeof(TupulMethod), 1);
	// method.name = tree.name;
	setupInterpretedMethod(method, tree.insns, clazz);
	// method.descr = tree.descr;
	// method[0].set(tree.name, tree.descr);
	method->name = tree.name;
	method->descr = tree.descr;
	return method;
}

MethodTree methodTreeFor(vector<byte> blockDescr, vector<byte> blockFunc) {
	string name = "";
	string descr = "";
	{
		//@formatter:on
		bool first = true;
		for (char c : blockDescr) {
			byte x = (byte) c;
			if (x == DESCRIPTOR) {
				first = false;
				continue;
			}
			if (first) name += c; else descr += c;
		}
		//@formatter:off
	}
	
	// println!();
	// println!("Method Name: {}\nDescriptor: {}", name.as_str(), descr.as_str());
	bool inBlock = false;
	bool second = false;
	vector<Insn> insns;
	byte opcode = 0;
	string arg0 = "";
	string arg1 = "";
	for (char c : blockFunc) {
		byte x = (byte) c;
		// if (x > 200) {
		//     //@formatter:off
		//          if (x == 253){printf("253 - descriptor\n")    ;}
		//     else if (x == 252){printf("252 - local\n")         ;}
		//     else if (x == 251){printf("251 - push\n")          ;}
		//     else if (x == 250){printf("250 - pop\n")           ;}
		//     else if (x == 249){printf("249 - loadc\n")         ;}
		//     else if (x == 248){printf("248 - setl\n")          ;}
		//     else if (x == 243){printf("243 - return\n")        ;}
		//     else if (x == 242){printf("242 - loadl\n")         ;}
		//     else if (x == 241){printf("241 - math\n")          ;}
		//     else              {printf("%i\n", x)               ;}
		//     //@formatter:on
		// }
		
		if (isOpcode(x)) {
			if (x != DESCRIPTOR) {
				if (opcode != 0) {
					if (inBlock) {
						if (second) {
							char* firstArg = (char*) calloc(sizeof(char), arg0.length() + 1);
							for (int i = 0; i < arg0.length(); i++) firstArg[i] = arg0[i];
							firstArg[arg0.length()] = 0;
							char* secondArg = (char*) calloc(sizeof(char), arg1.length() + 1);
							for (int i = 0; i < arg1.length(); i++) secondArg[i] = arg1[i];
							secondArg[arg1.length()] = 0;
							Insn insn = Insn { op: opcode, arg0: firstArg, arg1: secondArg };
							insns.push_back(insn);
							// println!("{:?}", insn.borrow());
							arg0 = "";
							arg1 = "";
							second = false;
							inBlock = false;
						} else {
							char* firstArg = (char*) calloc(sizeof(char), arg0.length() + 1);
							for (int i = 0; i < arg0.length(); i++) firstArg[i] = arg0[i];
							firstArg[arg0.length()] = 0;
							Insn insn = Insn { op: opcode, arg0: firstArg, arg1: nullptr };
							insns.push_back(insn);
							// println!("{:?}", insn);
							arg0 = "";
							inBlock = false;
						}
					} else {
						Insn insn = Insn { op: opcode, arg0: nullptr, arg1: nullptr };
						insns.push_back(insn);
						// println!("{:?}", insn);
						inBlock = false;
					}
					opcode = 0;
				}
			}
			if (is_expanded_opcode(x)) {
				inBlock = true;
				opcode = x;
				// println!("expanded: ");
				continue;
			} else if (x == DESCRIPTOR) {
				second = true;
				// println!("\ndescr: ");
				continue;
			} else {
				opcode = x;
			}
			continue;
		}
		if (second) {
			arg1 += c;
			// print!("{}", bytei(*x) as char)
		} else {
			arg0 += c;
			// print!("{}", bytei(*x) as char)
		}
	}
	
	if (opcode != 0) {
		if (inBlock) {
			if (second) {
				char* firstArg = (char*) calloc(sizeof(char), arg0.length() + 1);
				for (int i = 0; i < arg0.length(); i++) firstArg[i] = arg0[i];
				firstArg[arg0.length()] = 0;
				char* secondArg = (char*) calloc(sizeof(char), arg1.length() + 1);
				for (int i = 0; i < arg1.length(); i++) secondArg[i] = arg1[i];
				secondArg[arg1.length()] = 0;
				Insn insn = Insn { op: opcode, arg0: firstArg, arg1: secondArg };
				insns.push_back(insn);
				// println!("{:?}", insn.borrow());
			} else {
				char* firstArg = (char*) calloc(sizeof(char), arg0.length() + 1);
				for (int i = 0; i < arg0.length(); i++) firstArg[i] = arg0[i];
				firstArg[arg0.length()] = 0;
				Insn insn = Insn { op: opcode, arg0: firstArg, arg1: nullptr };
				insns.push_back(insn);
				// println!("{:?}", insn);
			}
		} else {
			Insn insn = Insn { op: opcode, arg0: nullptr, arg1: nullptr };
			insns.push_back(insn);
			// println!("{:?}", insn);
		}
		opcode = 0;
	}
	// println!();
	
	// Self { name, descr, insns }
	MethodTree tree;
	tree.name = name;
	tree.descr = descr;
	tree.insns = insns;
	return tree;
}