/* ntm.pl
 *
 * This module contains manipulations and a simulation of
 * a nondeterministic turing machine.
 *
 * Project: Turing-machine (FLP project 2)
 * Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
 * Last change: 25.04.2022
 */

:- module(ntm, [ states/2,
                 make_rules/1,
                 run/1,
                 tape/1
    ]).

:- use_module(io).

% Performes machine step based on the actual state and letter.
% Modiffies the tape. Returns configuration (tape)
% and all futured configurations. Calls next step.
% make_step(+state, +letter, +tape, -tape)
:- dynamic make_step/4.

% Remember turing machine configurations.
% Protects from easy looping.
% state_space(?tape)
:- dynamic state_space/1.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% States
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% initial(?char)
initial('S').

% final(?char)
final('F').

% states(+list_of_rules, -list_of_states)
states([], []).
states([[[S1], [_], [S2], [_]]|T], [S1|[S2|SS]]) :-states(T, SS).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Tape movements
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% left(?char)
left('L').

% right(?char)
right('R').

% movement(?char)
movement(M) :- left(M); right(M).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Tape symbols
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% blank(?char)
blank('#').

% letter(?char)
letter(A) :- char_type(A, lower); blank(A).

% state(?char)
state(S) :- char_type(S, upper).

% modification(?char)
modification(M) :- letter(M); movement(M).

% tape(?list_of_chars)
tape([_]).
tape([T|H]) :- letter(T), tape(H).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Rules
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Tests rule correctness.
% rule(?rule)
rule([[]]).
rule([[S1], [L1], [S2], [L2]]) :- state(S1), letter(L1), state(S2), modification(L2).

% Creates dynamic rules.
% make_rules(+list)
make_rules([]) :- !, fail.
make_rules([H]) :- rule(H), make_rule(H).
make_rules([H|T]) :- rule(H), make_rule(H), make_rules(T).

% Creates dynamic rule.
% make_rule(+rule)
make_rule([[S1], [L1], [S2], [L2]]) :- letter(L2), final(S2), !,
                                       assert((make_step(S1, L1, T, [T, NNT]) :-
                                            put_letter(L1, L2, S1, T, NT),
                                            change_state(S1, S2, NT, NNT)
                                       )).
make_rule([[S1], [L1], [S2], [L2]]) :-  letter(L2), !,
                                        assert((make_step(S1, L1, T, [T|O]) :-
                                            put_letter(L1, L2, S1, T, NT),
                                            change_state(S1, S2, NT, NNT),
                                            not(state_space(NNT)),
                                            assert(state_space(NNT)),
                                            make_step(S2, L2, NNT, O)
                                        )).
make_rule([[S1], [L1], [S2], [M]]) :-  movement(M), final(S2), !,
                                        assert((make_step(S1, L1, T, [T, NNT]) :-
                                            move(L1, S1, M, T, NT),
                                            change_state(S1, S2, NT, NNT)
                                        )).
make_rule([[S1], [L1], [S2], [M]]) :-  movement(M), !,
                                        assert((make_step(S1, L1, T, [T|O]) :-
                                            move(L1, S1, M, T, NT, L2),
                                            change_state(S1, S2, NT, NNT),
                                            not(state_space(NNT)),
                                            assert(state_space(NNT)),
                                            make_step(S2, L2, NNT, O)
                                        )).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Machine steps
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Makes left or right move based on the movement dirrection.
% Returns new tape and new letter under head.
% move(+letter, +state, +movement, +tape, -tape, -letter)
move(L1, S, M, T, NT, L2) :- (  left(M)
                             -> left_move(L1, S, T, NT, L2)
                             ;  right_move(L1, S, T, NT, L2)
                             ).

% Moves machine head to the left.
% Returns new tape and new letter under head.
% left_move(+letter, +state, +tape, -tape, -letter)
left_move(L1, S, [L2|[S|[L1|T]]], [S|[L2|[L1|T]]], L2).
left_move(L1, S, [L2|[S|[L1]]], [S|[L2|[L1]]], L2).
left_move(L1, S, [H|T], [H|TT], L2) :- left_move(L1, S, T, TT, L2).

% Moves machine head to the right.
% Returns new tape and new letter under head.
% right_move(+letter, +state, +tape, -tape, -letter)
right_move(L1, S, [S|[L1|[L2|T]]], [L1|[S|[L2|T]]], L2).
right_move(L1, S, [S|[L1]], [L1|[S|['#']]], '#').
right_move(L1, S, [H|T], [H|TT], L2) :- right_move(L1, S, T, TT, L2).

% Changes letter under head.
% Returns new tape.
% put_letter(+letter, +letter, +state, +tape, -tape)
put_letter(L1, L2, S, [S|[L1|T]], [S|[L2|T]]).
put_letter(L1, L2, S, [H|T], [H|TT]) :- put_letter(L1, L2, S, T, TT).

% Changes machine state.
% Returns new tape.
% change_state(+state, +state, +tape, -tape)
change_state(S1, S2, [S1|T], [S2|T]).
change_state(S1, S2, [H|T], [H|TT]) :- change_state(S1, S2, T, TT).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Run simulation
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Calls the first corresponding rule.
% run(+tape)
run([L|T]) :- !, make_step('S', L, ['S'|[L|T]], O), write_tapes(O).

% Write all configurations to the standard output.
% write_tapes(+list_of_tapes)
write_tapes([]).
write_tapes([H|T]) :- atom_chars(S, H), writeln(S), write_tapes(T).
