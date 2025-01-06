using JuMP
using GLPK

model = Model(GLPK.Optimizer)

#edges and capacities
edges = [(1, 2), (1, 3), (1, 5), (1, 9), (2, 4), (2, 6), (2, 10), (3, 4), (3, 7), (3, 11), (4, 8), (4, 12), (5, 6), (5, 7), (5, 13), (6, 8), (6, 14), (7, 8), (7, 15), (8, 16), (9, 10), (9, 11), (9, 13), (10, 12), (10, 14), (11, 12), (11, 15), (12, 16), (13, 14), (13, 15), (14, 16), (15, 16)]
capacities = Dict{Tuple{Int64, Int64}, Int64}(
	(1, 2) => 16,
	(1, 3) => 1,
	(1, 5) => 6,
	(1, 9) => 15,
	(2, 4) => 4,
	(2, 6) => 5,
	(2, 10) => 7,
	(3, 4) => 5,
	(3, 7) => 3,
	(3, 11) => 8,
	(4, 8) => 2,
	(4, 12) => 5,
	(5, 6) => 3,
	(5, 7) => 5,
	(5, 13) => 4,
	(6, 8) => 5,
	(6, 14) => 8,
	(7, 8) => 5,
	(7, 15) => 2,
	(8, 16) => 14,
	(9, 10) => 6,
	(9, 11) => 8,
	(9, 13) => 6,
	(10, 12) => 1,
	(10, 14) => 2,
	(11, 12) => 7,
	(11, 15) => 2,
	(12, 16) => 3,
	(13, 14) => 2,
	(13, 15) => 6,
	(14, 16) => 8,
	(15, 16) => 11)

#decision variables
@variable(model, flow[edges] >= 0)

#capacity constraints
for e in edges
	@constraint(model, flow[e] <= capacities[e])
end

#flow conservation constraints
for u in 1:16
	if u == 1 || u == 16
		continue
	end
	@constraint(model, sum(flow[(u,v)] for v in 1:16 if (u, v) in edges) == sum(flow[(v,u)] for v in 1:16 if (v, u) in edges))
end

#objective: maximize flow from source to sink
@objective(model, Max, sum(flow[(1, v)] for v in 1:16 if (1, v) in edges))

optimize!(model)

#print the results
if termination_status(model) == MOI.OPTIMAL
	println("maximum flow: ", objective_value(model))
	for e in edges
		println("flow[", e, "] = ", value(flow[e]))
	end
else
	println("no optimal solution")
end
