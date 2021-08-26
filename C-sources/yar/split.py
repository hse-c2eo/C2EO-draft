import os

for object in ['var', 'function']:
    out_dir = f'tests-{object}s'
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    with open(f'{object}s.c', 'r') as fin:
        for line in fin.readlines():
            if 'main()' in line or '//' in line:
                continue
            file_name = line
            while file_name and (file_name[-1] in ' ;{}()=\n' or file_name[-2:] == ' 1'):
                file_name = file_name[:-1]  # without ';'
            if not file_name:
                continue
            file_name = file_name.split()
            if object not in file_name[-1]:
                print([line, file_name])
                continue
            file_name = '_'.join([file_name[-1]] + file_name[:-1])
            file_name = f'{out_dir}/{file_name}.c'
            with open(file_name, 'w') as fout:
                print(line, file=fout)
                print('void main() {}', file=fout)
