# Parser Documentation

## checkOperationsTypes.cpp:

```
bool isBinaryOperator(TokenType token) - this function use vector 'binaryOperators' for checking if token is binary operator
bool isUnaryOperator(TokenType token) - this function use vector 'unaryOperators' for checking if token is unary operator

bool isNotOperator(TokenType token) - this function use vector 'notOperators' for checking if token is not operator
```

## nodeCreator.cpp

```
Node createNode(TokenType type, std::string value, int priority, OperationType operationType) - this function create node with given type, value, priority and operation type
```

## parser.cpp
