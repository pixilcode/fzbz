let fizzbuzz_file_name = ".fizzbuzz"

let program_error msg =
  print_endline msg;
  exit 1

let get_fizzbuzz_path () =
  let home_dir = Sys.getenv_opt "HOME" in
  match home_dir with
  | None -> program_error "error: $HOME environment variable isn't set"
  | Some home_dir -> Filename.concat home_dir fizzbuzz_file_name

let get_fizzbuzz_value loc =
  if not (Sys.file_exists loc) then None else
  let input_file = open_in loc in
  try
    let fizzbuzz_value = input_line input_file in
    close_in input_file;
    int_of_string_opt fizzbuzz_value
  with
  | End_of_file ->
    close_in input_file;
    None

let print_fizzbuzz value =
  let modulo_five = value mod 5 in
  let modulo_three = value mod 3 in
  if modulo_five != 0 && modulo_three != 0 then () else begin
    if modulo_three == 0 then print_string "fizz";
    if modulo_five == 0 then print_string "buzz";
    flush stdout;
    Unix.sleepf 0.3;
    print_string "\o033[2K\r";
    flush stdout
  end

let write_fizzbuzz loc value = 
  let output_file = open_out loc in
  output_string output_file (string_of_int value);
  close_out output_file

let () =
  let fizzbuzz_path = get_fizzbuzz_path () in
  let fizzbuzz_value = get_fizzbuzz_value fizzbuzz_path in
  let fizzbuzz_value = Option.value ~default:1 fizzbuzz_value in
  print_fizzbuzz fizzbuzz_value;
  write_fizzbuzz fizzbuzz_path (fizzbuzz_value + 1);
