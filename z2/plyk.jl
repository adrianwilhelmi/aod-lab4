using JuMP
using GLPK

model = Model(GLPK.Optimizer)

# edges in the bipartile graph
E = [(1, 5), (1, 6), (2, 7), (2, 6), (3, 7), (3, 6), (4, 8), (4, 7)]

# binary decision variables
@variable(model, x[E], Bin)

#define objective function: maximize sum of x for each edge in E
@objective(model, Max, sum(x[e] for e in E))
#constraint: each vertex in V1 can be part of at most one edge
for u in 1:4
	@constraint(model, sum(x[(u,v)] for v in 5:8 if (u,v) in E) <= 1)
end

#constraint: each vertex in v2 can be part of at most one edge
for v in 5:8
	@constraint(model, sum(x[(u,v)] for u in 1:4 if (u,v) in E) <= 1)
end

#optimize the model
optimize!(model)

#print the values of the decision variables
for e in E
	println("x[", e, "] = ", value(x[e]))
end

# save the model to file
write_to_file(model, "plyk.jl.lp")
