/* sim.pl
 *
 * This is the main module of FLP project.
 * Program reads turing machine from standard input
 * and runs the simulations on the input tape.
 * Machine configurations are printed to standard output
 * with every step of the machine.
 *
 * Exit codes: 0 - The turing machine accepts input tape.
 *             1 - The turing machine rejests input tape.
 *             2 - Bad input format or uncatchable infinite loop.
 *
 * Project: Turing-machine (FLP project 2)
 * Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
 * Last change: 25.04.2022
 */

:- use_module(io).  % read_lines/1, split_lines/2
:- use_module(ntm). % states/2, make_rules/1, run/1, tape/1


% Split the given array to the [1..n-1] and n.
% split_init_last(?Lines, ?Init, ?Last)
split_init_last([H], [], H).
split_init_last([H|T], [H|TT], L) :- split_init_last(T, TT, L).

main :-
    % Prints help if needed.
    current_prolog_flag(argv, A),
    (   (member('-h', A); member('--help', A))
    ->  write('USAGE:\nsim.pl <input >outptu\n'), halt(0)
    ;   true
    ),

    % Reads and splits the input.
    prompt(_, ''),
    read_lines(LL),
    split_lines(LL, S),
    split_init_last(S, R, [T]),

    % Checks rules format and makes dynamic predicates.
    (   make_rules(R)
    ->  true
    ;   writeln('ERROR: Bad turing machine format.'), halt(2)
    ), !,

    % Tests tape format.
    (   tape(T)
    ->  true
    ;   writeln('ERROR: Bad tape format.'), halt(2)
    ), !,

    % Test on initial state existence
    states(R, SS),
    (   member('S', SS)
    ->  true
    ;   writeln('ERROR: No initial state.'), halt(2)
    ), !,

    % The machine can immediately releft if there is no final state.
    (   member('F', SS)
    ->  true
    ;   halt(1)
    ), !,

    % Simulates machine run.
    (   run(T)
    ->  true
    ;   halt(1)
    ),

    halt(0).
