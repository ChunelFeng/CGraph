"""
Integration tests for the pipeline-based message tutorials.

Set ``PYCGRAPH_RUN_INTEGRATION=1`` and run against an installed wheel.
"""

import contextlib
import importlib.util
import io
import os
import pathlib
import sys
import unittest


PYTHON_ROOT = pathlib.Path(__file__).resolve().parents[1]
TUTORIAL_ROOT = PYTHON_ROOT / "tutorial"


def _load_tutorial(module_name, filename):
    spec = importlib.util.spec_from_file_location(
        module_name, TUTORIAL_ROOT / filename
    )
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


@unittest.skipUnless(
    os.environ.get("PYCGRAPH_RUN_INTEGRATION") == "1",
    "requires an installed pycgraph wheel",
)
class MessageTutorialIntegrationTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        sys.path.insert(0, str(TUTORIAL_ROOT))

    @classmethod
    def tearDownClass(cls):
        sys.path.remove(str(TUTORIAL_ROOT))

    def test_t16_runs_send_and_receive_nodes_in_separate_pipelines(self):
        from pycgraph import GNode
        from MyGNode.MyRecvMessageNode import MyRecvMessageNode
        from MyGNode.MySendMessageNode import MySendMessageNode

        self.assertTrue(issubclass(MySendMessageNode, GNode))
        self.assertTrue(issubclass(MyRecvMessageNode, GNode))
        tutorial = _load_tutorial(
            "tutorial_t16_message_send_recv",
            "T16-MessageSendRecv.py",
        )
        output = io.StringIO()

        with contextlib.redirect_stdout(output):
            tutorial.tutorial_message_send_recv()

        text = output.getvalue()
        self.assertIn("enter MyNode1 run function", text)
        self.assertIn("enter MyNode2 run function", text)
        self.assertIn("num = [0]", text)
        self.assertIn("num = [10]", text)
        self.assertIn("num = [20]", text)

    def test_t17_runs_one_publisher_and_three_subscriber_pipelines(self):
        from pycgraph import GNode
        from MyGNode.MyPubMessageNode import MyPubMessageNode
        from MyGNode.MySubMessageNode import MySubMessageNode

        self.assertTrue(issubclass(MyPubMessageNode, GNode))
        self.assertTrue(issubclass(MySubMessageNode, GNode))
        tutorial = _load_tutorial(
            "tutorial_t17_message_pub_sub",
            "T17-MessagePubSub.py",
        )
        output = io.StringIO()

        with contextlib.redirect_stdout(output):
            tutorial.tutorial_message_pub_sub()

        text = output.getvalue()
        self.assertEqual(3, text.count("[MySubMessageNode] [init]"))
        for num in (0, 100, 200, 300, 400):
            self.assertEqual(3, text.count("num = [{}]".format(num)))


if __name__ == "__main__":
    unittest.main()
